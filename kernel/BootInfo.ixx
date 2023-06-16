export module BootInfo;
import types;
import Framebuffer;
import Font;
import memory.Descriptor;
export
{
	struct BootInfo
	{
		Framebuffer* fb;
		PSF1Font* font;
		memory::Descriptor* map;
		u64 mapSize;
		u64 mapDescriptorSize;
	};
}