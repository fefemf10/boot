export module BootInfo;
import types;
import Framebuffer;
import Font;
import memory.descriptor;
import RSDP;
export
{
	struct MapEntry
	{
		void* address;
		u64 sizeOfBytes;
		u64 numberOfPages;
	};

	struct BootInfo
	{
		Framebuffer fb;
		PSF1Font* font;
		memory::Descriptor* map;
		u64 mapEntries;
		u64 mapSize;
		u64 descriptorSize;
		const static u64 numberOfMemoryMap = 5;
		MapEntry memoryMapEntries[numberOfMemoryMap];
		u64 rsv;
		ACPI::RSDP& RSDP;
		//stack kernel
		//kernel
		//bootinfo
		//font
		//VHD RAMDISK 32MB
	};
}