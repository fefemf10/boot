#include "..\uefilib\efi_tables.hpp"
#include "..\uefilib\efi_console.hpp"
#include "..\uefilib\efi_bootServices.hpp"
#include "..\uefilib\efi_runtimeServices.hpp"
#include "..\uefilib\efi_graphics.hpp"
#include "..\uefilib\efi_file.hpp"
import PE;
efi::SystemTable* SS;
efi::BootServices* BS;
efi::RuntimeServices* RS;
struct Framebuffer
{
	void* baseAddress;
	size_t bufferSize;
	uint32_t width;
	uint32_t height;
	uint32_t pixelsPerScanline;
	efi::GraphicsPixelFormat format;
};
Framebuffer framebuffer;

constexpr auto PSF1MAGIC0 = 0x36;
constexpr auto PSF1MAGIC1 = 0x04;
struct PSF1Font
{
	uint8_t magic[2];
	uint8_t mode;
	uint8_t charSize;
	uint8_t glyphBuffer[1];
};

void print(const char16_t* str)
{
	SS->conOut->outputString(SS->conOut, str);
}

extern "C" void putc(const char16_t c)
{
	const char16_t buf[2] = { c, u'\0' };
	SS->conOut->outputString(SS->conOut, buf);
}

extern "C" void printf_unsigned(uint64_t number, int32_t radix, int64_t width)
{
	int16_t buffer[32]{};
	int8_t pos = 0;
	const char16_t hexchar8_ts[] = u"0123456789ABCDEF";
	do
	{
		const uint64_t rem = number % radix;
		number /= radix;
		buffer[pos++] = hexchar8_ts[rem];
	} while (number);
	for (int8_t i = 0; i < width - pos; i++)
		SS->conOut->outputString(SS->conOut, u"0");
	while (--pos >= 0)
		putc(buffer[pos]);
	print(u"\n\r");
}

Framebuffer* InitializeGOP()
{
	efi::GUID gopGuid = efi::GRAPHICS_OUTPUT_PROTOCOL;
	efi::GraphicsOutputProtocol* gop;
	efi::Status status = BS->locateProtocol(&gopGuid, 0, (void**)&gop);
	if (efi::is_error(status))
	{
		SS->conOut->outputString(SS->conOut, u"Unable to locate GOP\n\r");
		return 0;
	}

	uint32_t requirenmentWidth = 1920;
	uint32_t requirenmentHeight = 1080;

	uint64_t sizeInfo{};
	efi::GraphicsOutputModeInformation* modeInformation{};
	uint64_t format{};
	for (uint64_t i = 0; i < gop->mode->maxMode; i++)
	{
		efi::Status s = gop->queryMode(gop, i, &sizeInfo, &modeInformation);
		if (s == efi::Status::SUCCESS && modeInformation->horizontalResolution <= requirenmentWidth && modeInformation->verticalResolution <= requirenmentHeight)
		{
			format = i;
		}
	}
	gop->setMode(gop, format);
	framebuffer.baseAddress = (void*)gop->mode->frameBufferBase;
	framebuffer.bufferSize = gop->mode->frameBufferSize;
	framebuffer.width = gop->mode->info->horizontalResolution;
	framebuffer.height = gop->mode->info->verticalResolution;
	framebuffer.pixelsPerScanline = gop->mode->info->pixelsPerScanLine;
	framebuffer.format = gop->mode->info->pixelFormat;
	return &framebuffer;
}

efi::FileHandle loadVolume(efi::Handle imageHandle)
{
	efi::LoadedImageProtocol* loadedImage{};
	efi::SimpleFileSystemProtocol* fileSystem{};
	efi::FileHandle volume;
	BS->handleProtocol(imageHandle, &efi::LOADED_IMAGE_PROTOCOL, (void**)&loadedImage);
	BS->handleProtocol(loadedImage->deviceHandle, &efi::SIMPLE_FILE_SYSTEM_PROTOCOL, (void**)&fileSystem);
	fileSystem->openVolume(fileSystem, &volume);
	return volume;
}

efi::FileHandle loadFile(efi::FileHandle directory, const char16_t* path, efi::Handle imageHangle)
{
	efi::FileHandle loadedFile;
	return directory->open(directory, &loadedFile, path, efi::FileMode::READ, efi::FileAttributes::FILE_READ_ONLY) == efi::Status::SUCCESS ? loadedFile : nullptr;
}
struct MapEntry
{
	void* address;
	uint64_t sizeOfBytes;
	uint64_t numberOfPages;
};

struct BootInfo
{
	Framebuffer fb;
	PSF1Font* font;
	efi::MemoryDescriptor* map;
	uint64_t mapEntries;
	uint64_t mapSize;
	uint64_t mapDescriptorSize;
	MapEntry memoryMapEntries[5];
	uint64_t rsv;
	void* ACPITable;
};

void (*mainCRTStartup)(BootInfo&);
extern void setStack(void* address, BootInfo* bootInfo);
extern uint64_t getStack();
constexpr uint64_t countPagesForInitAP = 5;
void* stackAddress = reinterpret_cast<void*>(0x1000 + 0x1000 * countPagesForInitAP);
uint64_t stackSize;
uint64_t numberOfPagesStack;
void* kernelAddress;
uint64_t kernelSize;
uint64_t numberOfPagesKernel;
void* bootInfoAddress;
uint64_t bootInfoSize;
uint64_t numberOfPagesBootInfo;
void* fontAddress;
uint64_t fontSize;
uint64_t numberOfPagesFont;
void* ramDiskAddress;
uint64_t ramDiskSize;
uint64_t numberOfPagesRamDisk;

constexpr const uint64_t BLOCKSIZE = 12;
constexpr uint64_t countPages(uint64_t size)
{
	return (size >> BLOCKSIZE) + (size % (1 << BLOCKSIZE) > 0);
}

PSF1Font* loadPSF1Font(efi::FileHandle directory, const char16_t* path, efi::Handle imageHangle)
{
	efi::FileHandle font = loadFile(directory, path, imageHangle);
	if (font == nullptr) return nullptr;
	efi::FileInfo* info;
	BS->allocatePool(efi::MemoryType::LOADER_CODE, sizeof(efi::FileInfo) + 128, (void**)&info);
	uint64_t sizeOfInfo = sizeof(efi::FileInfo) + 128;
	font->getInfo(font, &efi::FILE_INFO, &sizeOfInfo, info);
	fontSize = info->fileSize;
	BS->freePool(info);
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_CODE, numberOfPagesFont, fontAddress);
	
	PSF1Font* finishedFont = reinterpret_cast<PSF1Font*>(fontAddress);
	uint64_t sizeOfPSFHeader = 4;
	font->read(font, &sizeOfPSFHeader, finishedFont);
	if (finishedFont->magic[0] != PSF1MAGIC0 || finishedFont->magic[1] != PSF1MAGIC1)
		return nullptr;

	uint64_t glyphBufferSize = finishedFont->charSize * 256;
	if ((finishedFont->mode & 1) == 1)
	{
		glyphBufferSize *= 2;
	}
	glyphBufferSize += fontSize - glyphBufferSize - 4;
	font->read(font, &glyphBufferSize, finishedFont->glyphBuffer);
	return finishedFont;
}
void loadPE(efi::FileHandle file, PE::PE& pe)
{
	PE::Section sections[10];
	uint64_t sizeForRead;
	sizeForRead = sizeof(PE::MSDOSHeader);
	file->read(file, &sizeForRead, &pe.msdosHeader);
	file->setPosition(file, pe.msdosHeader.lfanew);
	sizeForRead = sizeof(PE::COFF);
	file->read(file, &sizeForRead, &pe.coffHeader);
	sizeForRead = sizeof(PE::OptionalHeader);
	file->read(file, &sizeForRead, &pe.optionalHeader);
	sizeForRead = sizeof(PE::OptionalHeaderData);
	file->read(file, &sizeForRead, &pe.optionalHeaderData);
	sizeForRead = sizeof(PE::Section);
	for (size_t i = 0; i < pe.coffHeader.numberSection; i++)
	{
		file->read(file, &sizeForRead, &sections[i]);
	}
	
	stackSize = pe.optionalHeader.sizeOfStackCommit;
	kernelSize = pe.optionalHeader.sizeOfImage;
	bootInfoSize = sizeof(BootInfo);

	numberOfPagesStack = countPages(stackSize);
	numberOfPagesKernel = countPages(kernelSize);
	numberOfPagesBootInfo = countPages(bootInfoSize);
	numberOfPagesFont = 3;
	

	kernelAddress = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(stackAddress) + (numberOfPagesStack << BLOCKSIZE));
	bootInfoAddress = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(kernelAddress) + (numberOfPagesKernel << BLOCKSIZE));
	fontAddress = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(bootInfoAddress) + (numberOfPagesBootInfo << BLOCKSIZE));
	ramDiskAddress = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(fontAddress) + (numberOfPagesFont << BLOCKSIZE));
	
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_CODE, numberOfPagesStack, stackAddress);
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_CODE, numberOfPagesKernel, kernelAddress);
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_CODE, numberOfPagesBootInfo, bootInfoAddress);

	file->setPosition(file, 0);
	sizeForRead = pe.optionalHeader.sizeOfHeaders;
	file->read(file, &sizeForRead, kernelAddress);
	for (size_t i = 0; i < pe.coffHeader.numberSection; i++)
	{
		sizeForRead = sections[i].virtualSize;
		if (sections[i].sizeRawData == 0)
		{
			continue;
		}
		void* sectionVirtualAddress = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(kernelAddress) + sections[i].virtualAddress);
		file->setPosition(file, sections[i].pointerRawData);
		file->read(file, &sizeForRead, sectionVirtualAddress);
	}
	if (pe.optionalHeaderData.baseRelocationTable.size)
	{
		uint64_t offset = 0;
		while (offset < pe.optionalHeaderData.baseRelocationTable.size)
		{
			uint32_t rva = *reinterpret_cast<u32*>(reinterpret_cast<uint64_t>(kernelAddress) + pe.optionalHeaderData.baseRelocationTable.virtualAddress + offset);
			uint32_t sizeOfBlock = *reinterpret_cast<u32*>(reinterpret_cast<uint64_t>(kernelAddress) + pe.optionalHeaderData.baseRelocationTable.virtualAddress + offset + 4);
			//printf_unsigned(sizeOfBlock, 10, 0);
			//SS->conOut->outputString(SS->conOut, u" ");
			uint64_t entries = (sizeOfBlock - 8) / 2;
			u16* relocationVirtualTable = reinterpret_cast<u16*>(reinterpret_cast<uint64_t>(kernelAddress) + pe.optionalHeaderData.baseRelocationTable.virtualAddress + offset + 8);
			for (size_t i = 0; i < entries; i++)
			{
				if (relocationVirtualTable[i] == 0)
					continue;
				*reinterpret_cast<u64*>(reinterpret_cast<uint64_t>(kernelAddress) + rva + (relocationVirtualTable[i] & 0x0FFF)) += reinterpret_cast<uint64_t>(kernelAddress) - pe.optionalHeader.imageBase;
				//printf_unsigned(rva + (relocationVirtualTable[i] & 0x0FFF), 16, 0);
				//SS->conOut->outputString(SS->conOut, u" ");
			}
			offset += sizeOfBlock;
			//printf_unsigned(offset, 10, 0);
			//SS->conOut->outputString(SS->conOut, u"\n\r");
		}
	}
	mainCRTStartup = reinterpret_cast<void(*)(BootInfo&)>(reinterpret_cast<uint64_t>(kernelAddress) + pe.optionalHeader.addressOfEntryPoint);
}
const char16_t* EFI_MEMORY_TYPE_STRINGS[]{

	u"EfiReservedMemoryType",
	u"EfiLoaderCode",
	u"EfiLoaderData",
	u"EfiBootServicesCode",
	u"EfiBootServicesData",
	u"EfiRuntimeServicesCode",
	u"EfiRuntimeServicesData",
	u"EfiConventionalMemory",
	u"EfiUnusableMemory",
	u"EfiACPIReclaimMemory",
	u"EfiACPIMemoryNVS",
	u"EfiMemoryMappedIO",
	u"EfiMemoryMappedIOPortSpace",
	u"EfiPalCode",
};

uint64_t getMemorySize(efi::MemoryDescriptor* map, uint64_t mapEntries, uint64_t descriptorSize)
{
	static uint64_t memorySizeBytes{};
	if (memorySizeBytes > 0) return memorySizeBytes;
	for (size_t i = 0; i < mapEntries; i++)
	{
		efi::MemoryDescriptor* descriptor = (efi::MemoryDescriptor*)((uint64_t)map + (i * descriptorSize));
		memorySizeBytes += descriptor->numberOfPages * 0x1000;
	}
	return memorySizeBytes;
}


efi::Status efi_main(efi::Handle imageHandle, efi::SystemTable* systemTable)
{
	SS = systemTable;
	BS = systemTable->bootServices;
	RS = systemTable->runtimeServices;
	efi::FileHandle volume = loadVolume(imageHandle);
	Framebuffer* newBuffer = InitializeGOP();
	
	efi::FileHandle apinitFile = loadFile(volume, u"apinit.bin", imageHandle);
	size_t readSize = 256;
	apinitFile->read(apinitFile, &readSize, (void*)(0x1000));
	efi::FileHandle kernel = loadFile(volume, u"kernel.exe", imageHandle);
	PE::PE kernelPE;
	loadPE(kernel, kernelPE);
	PSF1Font* newFont = loadPSF1Font(volume, u"zap-ext-vga16.psf", imageHandle);
	efi::FileHandle ramDiskFile = loadFile(volume, u"ramdisk.vhd", imageHandle);
	efi::FileInfo* info;
	BS->allocatePool(efi::MemoryType::LOADER_CODE, sizeof(efi::FileInfo) + 128, (void**)&info);
	uint64_t sizeOfInfo = sizeof(efi::FileInfo) + 128;
	ramDiskFile->getInfo(ramDiskFile, &efi::FILE_INFO, &sizeOfInfo, info);
	ramDiskSize = info->fileSize;
	BS->freePool(info);
	readSize = ramDiskSize;
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_CODE, numberOfPagesRamDisk, ramDiskAddress);
	ramDiskFile->read(ramDiskFile, &readSize, ramDiskAddress);
	numberOfPagesRamDisk = countPages(ramDiskSize);
	efi::MemoryDescriptor* map{};
	uint64_t mapSize{}, mapKey{};
	uint64_t descriptorSize{};
	uint32_t descriptorVersion{};
	BS->getMemoryMap(&mapSize, map, &mapKey, &descriptorSize, &descriptorVersion);
	mapSize += 2 * descriptorSize;
	BS->allocatePool(efi::MemoryType::LOADER_CODE, mapSize, (void**)&map);
	BS->getMemoryMap(&mapSize, map, &mapKey, &descriptorSize, &descriptorVersion);
	uint64_t mapEntries = mapSize / descriptorSize;

	BootInfo bootInfo;
	bootInfo.fb = framebuffer;
	bootInfo.font = newFont;
	bootInfo.map = map;
	bootInfo.mapEntries = mapEntries;
	bootInfo.mapSize = mapSize;
	bootInfo.mapDescriptorSize = descriptorSize;
	//stack
	bootInfo.memoryMapEntries[0].address = stackAddress;
	bootInfo.memoryMapEntries[0].sizeOfBytes = stackSize;
	bootInfo.memoryMapEntries[0].numberOfPages = numberOfPagesStack;
	//kernel
	bootInfo.memoryMapEntries[1].address = kernelAddress;
	bootInfo.memoryMapEntries[1].sizeOfBytes = kernelSize;
	bootInfo.memoryMapEntries[1].numberOfPages = numberOfPagesKernel;
	//bootInfo
	bootInfo.memoryMapEntries[2].address = bootInfoAddress;
	bootInfo.memoryMapEntries[2].sizeOfBytes = bootInfoSize;
	bootInfo.memoryMapEntries[2].numberOfPages = numberOfPagesBootInfo;
	//font
	bootInfo.memoryMapEntries[3].address = fontAddress;
	bootInfo.memoryMapEntries[3].sizeOfBytes = fontSize;
	bootInfo.memoryMapEntries[3].numberOfPages = numberOfPagesFont;
	//VHD RAMDISK 32MB
	bootInfo.memoryMapEntries[4].address = ramDiskAddress;
	bootInfo.memoryMapEntries[4].sizeOfBytes = ramDiskSize;
	bootInfo.memoryMapEntries[4].numberOfPages = numberOfPagesRamDisk;

	for (size_t i = 0; i < SS->numberOfTableEntries; i++)
	{
		if (SS->configurationTables[i].vendorGUID == efi::ACPI_20)
		{
			bootInfo.ACPITable = SS->configurationTables[i].vendorTable;
			break;
		}
	}

	*reinterpret_cast<BootInfo*>(bootInfoAddress) = bootInfo;
	BS->exitBootServices(imageHandle, mapKey);

	setStack(reinterpret_cast<void*>(reinterpret_cast<uint64_t>(stackAddress) + stackSize), reinterpret_cast<BootInfo*>(bootInfoAddress));

	return efi::Status::SUCCESS;
}
