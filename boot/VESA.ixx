export module VESA;
import VESA.structures;
import types;
import memory;
import console;
export namespace VESA
{
	VESAInfo* vesaInfo;
	VESAModeInfo* vesaModesInfo;
	VESAModeInfo vesaMode;
	u32 countModes;
	u16 currentMode;
	
	void initialize()
	{
		vesaInfo = reinterpret_cast<VESAInfo*>(VESAInfo::address);
		countModes = *reinterpret_cast<u32*>(VESAModeInfo::address);
		currentMode = *reinterpret_cast<u16*>(VESAModeInfo::address + 4);
		vesaModesInfo = reinterpret_cast<VESAModeInfo*>(VESAModeInfo::address + 6);
		memory::pageTableManager.mapMemory(vesaModesInfo, vesaModesInfo);
		vesaMode = vesaModesInfo[currentMode];
		u32 size = vesaMode.height * vesaMode.pitch;
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(vesaMode.framebuffer), reinterpret_cast<void*>(vesaMode.framebuffer), size);
	}
	void drawPixel(u16 x, u16 y, u8 r, u8 g, u8 b)
	{
		u32 pixel_offset = y * vesaMode.pitch + x * vesaMode.bpp / 8;
		reinterpret_cast<u8*>(vesaMode.framebuffer)[pixel_offset + 0] = b;
		reinterpret_cast<u8*>(vesaMode.framebuffer)[pixel_offset + 1] = g;
		reinterpret_cast<u8*>(vesaMode.framebuffer)[pixel_offset + 2] = r;
	}
}