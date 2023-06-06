#include "..\uefilib\efi_tables.hpp"
#include "..\uefilib\efi_console.hpp"
#include "..\uefilib\efi_bootServices.hpp"
#include "..\uefilib\efi_runtimeServices.hpp"
#include "..\uefilib\efi_graphics.hpp"
#include "..\uefilib\efi_file.hpp"
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
struct Point
{
	uint32_t x;
	uint32_t y;
} cursorPos{};
void putChar(Framebuffer* fb, PSF1Font* font, uint32_t color, char8_t c, uint32_t offsetX, uint32_t offsetY)
{
	uint32_t* pixels = (uint32_t*)fb->baseAddress;
	char8_t* fontPtr = reinterpret_cast<char8_t*>(font->glyphBuffer) + (c * font->psf1Header->charSize);
	for (size_t y = offsetY; y < offsetY + 16; y++)
	{
		for (size_t x = offsetX; x < offsetX + 8; x++)
		{
			if ((*fontPtr & (0b10000000 >> (x-offsetX))) > 0)
			{
				*(uint32_t*)(pixels + x + (y * fb->pixelsPerScanline)) = color;
			}
		}
		++fontPtr;
	}
}

void print(Framebuffer* fb, PSF1Font* font, const char8_t* str)
{
	const char8_t* c = str;
	while (*c)
	{
		putChar(fb, font, 0x00FFFFFF, *c, cursorPos.x, cursorPos.y);
		cursorPos.x += 8;
		if (cursorPos.x + 8 > fb->width)
		{
			cursorPos.x = 0;
			cursorPos.y += 16;
		}
		++c;
	}
}
bool GrowBuffer(efi::Status* status, void** buffer, uint64_t bufferSize)
{
	bool         TryAgain;

	if (!*buffer && bufferSize)
	{
		*status = efi::Status::BUFFER_TOO_SMALL;
	}

	TryAgain = false;
	if (*status == efi::Status::BUFFER_TOO_SMALL)
	{

		if (*buffer)
		{
			BS->freePool(*buffer);
		}

		BS->allocatePool(efi::MemoryType::LOADER_DATA,bufferSize, buffer);

		if (*buffer)
		{
			TryAgain = true;
		}
		else
		{
			*status = efi::Status::OUT_OF_RESOURCES;
		}
	}

	if (!TryAgain && efi::is_error(*status) && *buffer) {
		BS->freePool(*buffer);
		*buffer = nullptr;
	}

	return TryAgain;
}
efi::FileInfo* getFileInfo(efi::FileHandle handle)
{
	efi::Status status{};
	efi::FileInfo* info{};
	uint64_t bufferSize = ((uint64_t)(int64_t)(&(((efi::FileInfo*)0)->fileName))) + 200;
	while (GrowBuffer(&status, (void**)&info, bufferSize)) {
		status = handle->getInfo(handle,&efi::FILE_INFO,&bufferSize,info);
	}
	return info;
}
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
	const char8_t* d = u8"я ты она";
	for (size_t j = 0; j < 37; j++)
	for (size_t i = 0; i < 100; i++)
	{
		print(newBuffer, newFont, u8"a");
	}

	efi::FileHandle kernel = loadFile(volume, u"kernel.exe", imageHandle);
	if (kernel == nullptr)
	{
		systemTable->conOut->outputString(systemTable->conOut, u"Counld not load kernel\n\r");
	}
	else
	{
		systemTable->conOut->outputString(systemTable->conOut, u"Kernel loaded successfully\n\r");
	}
	/*systemTable->conIn->reset(systemTable->conIn, false);
	systemTable->bootServices->waitForEvent(1, &systemTable->conIn->waitForKey, &event);
	systemTable->runtimeServices->resetSystem(efi::ResetType::RESET_WARM, efi::Status::SUCCESS, 0, nullptr);*/
	return efi::Status::SUCCESS;
}
