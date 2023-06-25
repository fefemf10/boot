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
	for (uint64_t i = 0; i < gop->mode->maxMode; i++)
	{
		efi::Status s = gop->queryMode(gop, i, &sizeInfo, &modeInformation);
		if (s == efi::Status::SUCCESS && modeInformation->horizontalResolution >= requirenmentWidth && modeInformation->verticalResolution >= requirenmentHeight)
		{
			gop->setMode(gop, i);
			break;
		}
	}

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


struct BootInfo
{
	Framebuffer* fb;
	PSF1Font* font;
	efi::MemoryDescriptor* map;
	uint64_t mapEntries;
	uint64_t mapSize;
	uint64_t mapDescriptorSize;
	void* kernelAddress;
	uint64_t kernelSize;
	uint64_t kernelStackSize;
	uint64_t kernelResourcesSize;
};

void (*mainCRTStartup)(BootInfo);
extern void setStack(uint64_t address, BootInfo* bootInfo);
extern uint64_t getStack();
void* image = reinterpret_cast<void*>(0x1000);
PSF1Font* loadPSF1Font(BootInfo& bootInfo, efi::FileHandle directory, const char16_t* path, efi::Handle imageHangle)
{
	constexpr const uint64_t countPagesForFont = 3;
	efi::FileHandle font = loadFile(directory, path, imageHangle);
	if (font == nullptr) return nullptr;
	void* fontStruct = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(image) + bootInfo.kernelSize);
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_DATA, countPagesForFont, fontStruct);
	PSF1Font* finishedFont = reinterpret_cast<PSF1Font*>(fontStruct);
	uint64_t size = 4;
	font->read(font, &size, finishedFont);
	if (finishedFont->magic[0] != PSF1MAGIC0 || finishedFont->magic[1] != PSF1MAGIC1)
		return nullptr;
	uint64_t glyphBufferSize = finishedFont->charSize * 256;
	if (finishedFont->mode == 1)
	{
		glyphBufferSize *= 2;
	}
	font->read(font, &glyphBufferSize, finishedFont->glyphBuffer);
	bootInfo.kernelResourcesSize += countPagesForFont * 0x1000;
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
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_CODE, (pe.optionalHeader.sizeOfImage >> 12) + (pe.optionalHeader.sizeOfStackCommit >> 12), image);
	image = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(image) + pe.optionalHeader.sizeOfStackCommit);
	file->setPosition(file, 0);
	sizeForRead = pe.optionalHeader.sizeOfHeaders;
	file->read(file, &sizeForRead, image);
	for (size_t i = 0; i < pe.coffHeader.numberSection; i++)
	{
		sizeForRead = sections[i].virtualSize;
		if (sections[i].sizeRawData == 0)
		{
			continue;
		}
		void* sectionVirtualAddress = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(image) + sections[i].virtualAddress);
		file->setPosition(file, sections[i].pointerRawData);
		file->read(file, &sizeForRead, sectionVirtualAddress);
	}
	if (pe.optionalHeaderData.baseRelocationTable.size)
	{
		uint64_t offset = 0;
		while (offset < pe.optionalHeaderData.baseRelocationTable.size)
		{
			uint32_t rva = *reinterpret_cast<u32*>(reinterpret_cast<uint64_t>(image) + pe.optionalHeaderData.baseRelocationTable.virtualAddress + offset);
			uint32_t sizeOfBlock = *reinterpret_cast<u32*>(reinterpret_cast<uint64_t>(image) + pe.optionalHeaderData.baseRelocationTable.virtualAddress + offset + 4);
			//printf_unsigned(sizeOfBlock, 10, 0);
			//SS->conOut->outputString(SS->conOut, u" ");
			uint64_t entries = (sizeOfBlock - 8) / 2;
			u16* relocationVirtualTable = reinterpret_cast<u16*>(reinterpret_cast<uint64_t>(image) + pe.optionalHeaderData.baseRelocationTable.virtualAddress + offset + 8);
			for (size_t i = 0; i < entries; i++)
			{
				if (relocationVirtualTable[i] == 0)
					continue;
				*reinterpret_cast<u64*>(reinterpret_cast<uint64_t>(image) + rva + (relocationVirtualTable[i] & 0x0FFF)) += reinterpret_cast<uint64_t>(image) - pe.optionalHeader.imageBase;
				//printf_unsigned(rva + (relocationVirtualTable[i] & 0x0FFF), 16, 0);
				//SS->conOut->outputString(SS->conOut, u" ");
			}
			offset += sizeOfBlock;
			//printf_unsigned(offset, 10, 0);
			//SS->conOut->outputString(SS->conOut, u"\n\r");
		}
	}
	mainCRTStartup = reinterpret_cast<void(*)(BootInfo)>(reinterpret_cast<uint64_t>(image) + pe.optionalHeader.addressOfEntryPoint);
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
	uint64_t event;
	efi::FileHandle volume = loadVolume(imageHandle);
	Framebuffer* newBuffer = InitializeGOP();
	systemTable->conOut->clearScreen(systemTable->conOut);

	efi::FileHandle kernel = loadFile(volume, u"kernel.exe", imageHandle);
	PE::PE kernelPE;
	loadPE(kernel, kernelPE);

	efi::MemoryDescriptor* map{};
	uint64_t mapSize, mapKey;
	uint64_t descriptorSize;
	uint32_t descriptorVersion;
	BS->getMemoryMap(&mapSize, map, &mapKey, &descriptorSize, &descriptorVersion);
	BS->allocatePool(efi::MemoryType::LOADER_DATA, mapSize + 2 * descriptorSize, (void**)&map);
	BS->getMemoryMap(&mapSize, map, &mapKey, &descriptorSize, &descriptorVersion);

	uint64_t mapEntries = mapSize / descriptorSize;
	/*for (size_t i = 0; i < mapEntries; i++)
	{
		efi::MemoryDescriptor* descriptor = reinterpret_cast<efi::MemoryDescriptor*>((uint64_t)map + (i * descriptorSize));
		systemTable->conOut->outputString(systemTable->conOut, EFI_MEMORY_TYPE_STRINGS[descriptor->type]);
	}*/

	BootInfo bootInfo;
	bootInfo.fb = &framebuffer;
	bootInfo.map = map;
	bootInfo.mapEntries = mapEntries;
	bootInfo.mapSize = mapSize;
	bootInfo.mapDescriptorSize = descriptorSize;
	bootInfo.kernelAddress = image;
	bootInfo.kernelSize = kernelPE.optionalHeader.sizeOfImage;
	bootInfo.kernelStackSize = kernelPE.optionalHeader.sizeOfStackCommit;
	PSF1Font* newFont = loadPSF1Font(bootInfo, volume, u"zap-ext-vga16.psf", imageHandle);
	bootInfo.font = newFont;
	
	BS->exitBootServices(imageHandle, mapKey);
	setStack(reinterpret_cast<uint64_t>(image), &bootInfo);

	return efi::Status::SUCCESS;
}
