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
		vesaMode = vesaModesInfo[currentMode];
		u8* phys = reinterpret_cast<u8*>(vesaMode.framebuffer);
		u32 size = vesaMode.height * vesaMode.pitch;
		for (; phys < reinterpret_cast<u8*>(vesaMode.framebuffer + size); phys += 0x1000)
		{
			memory::pageTableManager.mapMemory(phys, phys);
		}
		
	}
	void drawPixel(u16 x, u16 y, u8 r, u8 g, u8 b)
	{
		u32 pixel_offset = y * vesaMode.pitch + x * vesaMode.bpp / 8;
		reinterpret_cast<u8*>(vesaMode.framebuffer)[0] = r;
		reinterpret_cast<u8*>(vesaMode.framebuffer)[1] = g;
		reinterpret_cast<u8*>(vesaMode.framebuffer)[2] = b;
	}
}