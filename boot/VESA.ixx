export module VESA;
import VESA.structures;
import types;
import memory;
export namespace VESA
{

	VESAInfo* vesaInfo;
	VESAModeInfo* vesaModesInfo;
	u32 countModes;
	u16 currentMode;
	void initialize()
	{
		vesaInfo = reinterpret_cast<VESAInfo*>(VESAInfo::address);
		countModes = *reinterpret_cast<u32*>(VESAModeInfo::address);
		currentMode = *reinterpret_cast<u16*>(VESAModeInfo::address + 4);
		vesaModesInfo = reinterpret_cast<VESAModeInfo*>(VESAModeInfo::address + 6);
		for (size_t i = 0; i < vesaModesInfo[currentMode].width * vesaModesInfo[currentMode].height * vesaModesInfo[currentMode].bpp/8; i+=0x1000)
		{
			memory::pageTableManager.mapMemory((u64*)vesaModesInfo[currentMode].framebuffer + i, (u64*)vesaModesInfo[currentMode].framebuffer + i);
		}
	}
	void drawPixel(u16 x, u16 y, u8 r, u8 g, u8 b)
	{
		u32 pixel_offset = y * vesaModesInfo[currentMode].pitch + (x * (vesaModesInfo[currentMode].bpp / 8)) + vesaModesInfo[currentMode].framebuffer;
		reinterpret_cast<u8*>(pixel_offset)[0] = r;
		reinterpret_cast<u8*>(pixel_offset)[1] = g;
		reinterpret_cast<u8*>(pixel_offset)[2] = b;
	}
}