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
};
Framebuffer framebuffer;

struct PSF1Header
{
	uint8_t magic[2];
	uint8_t mode;
	uint8_t charSize;
};
constexpr auto PSF1MAGIC0 = 0x36;
constexpr auto PSF1MAGIC1 = 0x04;
struct PSF1Font
{
	PSF1Header* psf1Header;
	void* glyphBuffer;
};

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
	else
	{
		SS->conOut->outputString(SS->conOut, u"GOP locateed\n\r");
	}
	framebuffer.baseAddress = (void*)gop->mode->frameBufferBase;
	framebuffer.bufferSize = gop->mode->frameBufferSize;
	framebuffer.width = gop->mode->info->horizontalResolution;
	framebuffer.height = gop->mode->info->verticalResolution;
	framebuffer.pixelsPerScanline = gop->mode->info->pixelsPerScanLine;
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
PSF1Font* loadPSF1Font(efi::FileHandle directory, const char16_t* path, efi::Handle imageHangle)
{
	efi::FileHandle font = loadFile(directory, path, imageHangle);
	if (font == nullptr) return nullptr;
	PSF1Header* fontHeader;
	BS->allocatePool(efi::MemoryType::LOADER_DATA, sizeof(PSF1Header), (void**)&fontHeader);
	uint64_t size = sizeof(PSF1Header);
	font->read(font, &size, fontHeader);
	if (fontHeader->magic[0] != PSF1MAGIC0 || fontHeader->magic[1] != PSF1MAGIC1)
		return nullptr;
	uint64_t glyphBufferSize = fontHeader->charSize * 256;
	if (fontHeader->mode == 1)
	{
		glyphBufferSize *= 2;
	}
	void* glyphBuffer;
	{
		font->setPosition(font, sizeof(PSF1Header));
		BS->allocatePool(efi::MemoryType::LOADER_DATA, glyphBufferSize, (void**)&glyphBuffer);
		font->read(font, &glyphBufferSize, glyphBuffer);
	}
	PSF1Font* finishedFont;
	BS->allocatePool(efi::MemoryType::LOADER_DATA, sizeof(PSF1Font), (void**)(&finishedFont));
	finishedFont->psf1Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;
}


struct BootInfo
{
	Framebuffer* fb;
	PSF1Font* font;
	efi::MemoryDescriptor* map;
	uint64_t mapSize;
	uint64_t mapDescriptorSize;
};

int (*mainCRTStartup)();

void loadPE(efi::FileHandle file, PE::PE& pe)
{
	void* image;
	
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
	BS->allocatePages(efi::AllocateType::ALLOCATE_ADDRESS, efi::MemoryType::LOADER_CODE, pe.optionalHeader.sizeOfImage >> 12, image);
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
		uint64_t entries = (pe.optionalHeaderData.baseRelocationTable.size - 8) / 2 - 1;
		uint32_t rva = *reinterpret_cast<u32*>(reinterpret_cast<uint64_t>(image) + pe.optionalHeaderData.baseRelocationTable.virtualAddress);
		u16* relocationVirtualTable = reinterpret_cast<u16*>(reinterpret_cast<uint64_t>(image) + pe.optionalHeaderData.baseRelocationTable.virtualAddress + 8);
		for (size_t i = 0; i < entries; i++)
		{
			*reinterpret_cast<u64*>(reinterpret_cast<uint64_t>(image) + rva + (relocationVirtualTable[i] & 0x0FFF)) += reinterpret_cast<uint64_t>(image) - pe.optionalHeader.imageBase;
		}
	}
	mainCRTStartup = reinterpret_cast<int (*)()>(reinterpret_cast<uint64_t>(image) + pe.optionalHeader.addressOfEntryPoint);
}
const char16_t* EFI_MEMORY_TYPE_STRINGS[]{

	u"EfiReservedMemoryType\n\r",
	u"EfiLoaderCode\n\r",
	u"EfiLoaderData\n\r",
	u"EfiBootServicesCode\n\r",
	u"EfiBootServicesData\n\r",
	u"EfiRuntimeServicesCode\n\r",
	u"EfiRuntimeServicesData\n\r",
	u"EfiConventionalMemory\n\r",
	u"EfiUnusableMemory\n\r",
	u"EfiACPIReclaimMemory\n\r",
	u"EfiACPIMemoryNVS\n\r",
	u"EfiMemoryMappedIO\n\r",
	u"EfiMemoryMappedIOPortSpace\n\r",
	u"EfiPalCode\n\r",
};
efi::Status efi_main(efi::Handle imageHandle, efi::SystemTable* systemTable)
{
	SS = systemTable;
	BS = systemTable->bootServices;
	RS = systemTable->runtimeServices;
	uint64_t event;
	systemTable->conOut->clearScreen(systemTable->conOut);
	systemTable->conOut->outputString(systemTable->conOut, u"Init kernel\n\r");
	efi::FileHandle volume = loadVolume(imageHandle);
	PSF1Font* newFont = loadPSF1Font(volume, u"zap-ext-vga16.psf", imageHandle);
	if (newFont == nullptr)
	{
		systemTable->conOut->outputString(systemTable->conOut, u"Counld not load font\n\r");
	}
	Framebuffer* newBuffer = InitializeGOP();


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

	/*uint64_t mapEntries = mapSize / descriptorSize;
	for (size_t i = 0; i < mapEntries; i++)
	{
		efi::MemoryDescriptor* descriptor = reinterpret_cast<efi::MemoryDescriptor*>((uint64_t)map + (i * descriptorSize));
		systemTable->conOut->outputString(systemTable->conOut, EFI_MEMORY_TYPE_STRINGS[descriptor->type]);
	}*/

	BootInfo bootInfo;
	bootInfo.fb = &framebuffer;
	bootInfo.font = newFont;
	bootInfo.map = map;
	bootInfo.mapSize = mapSize;
	bootInfo.mapDescriptorSize = descriptorSize;

	//BS->exitBootServices(imageHandle, mapKey);

	if (mainCRTStartup() == 'b')
	{
		SS->conOut->outputString(SS->conOut, u"Counld not load kernel\n\r");
	}
	else
	{
		SS->conOut->outputString(SS->conOut, u"Kernel loaded successfully\n\r");
	}
	systemTable->conIn->reset(systemTable->conIn, false);
	systemTable->bootServices->waitForEvent(1, &systemTable->conIn->waitForKey, &event);
	systemTable->runtimeServices->resetSystem(efi::ResetType::RESET_WARM, efi::Status::SUCCESS, 0, nullptr);
	return efi::Status::SUCCESS;
}
