export module BootInfo;
import types;
import Framebuffer;
import Font;
import memory.descriptor;
export
{
	struct BootInfo
	{
		Framebuffer* fb;
		PSF1Font* font;
		memory::Descriptor* map;
		u64 mapEntries;
		u64 mapSize;
		u64 descriptorSize;
		void* kernelAddress;
		u64 kernelSize;
		u64 kernelStackSize;
		u64 kernelResourcesSize;
	};
}