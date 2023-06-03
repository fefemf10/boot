#include "..\uefilib\efi.hpp"
typedef struct
{
	void* BaseAddress;
	size_t BufferSize;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanline;
} Framebuffer;
Framebuffer framebuffer;
Framebuffer* InitializeGOP()
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, 0, (void**)&gop);
	if (EFI_ERROR(status))
	{
		Print(L"Unable to locate GOP\n\r");
		return 0;
	}
	else
	{
		Print(L"GOP locateed\n\r");
	}
	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanline = gop->Mode->Info->PixelsPerScanLine;
	return &framebuffer;
}

EFI_FILE* LoadFile(EFI_FILE* Directory, const wchar_t* Path, EFI_HANDLE ImageHangle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* LoadedFile;
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHangle, &gEfiLoadedImageProtocolGuid, (void**)(&LoadedImage));
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)(&FileSystem));
	if (!Directory)
		FileSystem->OpenVolume(FileSystem, &Directory);
	EFI_STATUS status = Directory->Open(Directory, &LoadedFile, (wchar_t*)(Path), EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	return status == EFI_SUCCESS ? LoadedFile : 0;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	UINTN Event;
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	wchar_t buf[] = L"TestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTest";
	//SystemTable->ConOut->OutputString(SystemTable->ConOut, L"asd");
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, 0, 0, 0);
	/*UINTN Event;
	InitializeLib(ImageHandle, SystemTable);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	Print(L"Hello UEFI\n\r");
	Framebuffer* newBuffer = InitializeGOP();
	Print(L"Base: %x\n\rSize: %d\n\rWidth: %d\n\rHeight: %d\n\rPixelsPerScanline: %d\n\r",
		newBuffer->BaseAddress, newBuffer->BufferSize, newBuffer->Width, newBuffer->Height, newBuffer->PixelsPerScanline);
	EFI_FILE* kernle = LoadFile(0, L"kernel.exe", ImageHandle, SystemTable);
	if (kernle == 0)
	{
		Print(L"Counld not load kernel\n\r");
	}
	else
	{
		Print(L"Kernel loaded successfully\n\r");
	}
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);*/
	return 0;
}
