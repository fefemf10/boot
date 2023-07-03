export module BootInfo;
import types;
import Framebuffer;
import Font;
import memory.descriptor;
export
{
	struct MapEntry
	{
		void* address;
		u64 sizeOfBytes;
		u64 numberOfPages;
	};
#pragma pack(16)
	struct BootInfo
	{
		Framebuffer fb;
		PSF1Font* font;
		memory::Descriptor* map;
		u64 mapEntries;
		u64 mapSize;
		u64 descriptorSize;
		MapEntry memoryMapEntries[4];
		//stack kernel
		//kernel
		//bootinfo
		//font
	};
}