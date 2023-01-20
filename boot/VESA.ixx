export module VESA;
import VESA.structures;
import types;
import memory;
import console;
import math;
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
	void drawLine(i16 x0, i16 y0, i16 x1, i16 y1, u8 r, u8 g, u8 b)
	{
		bool step{};
		if (math::abs(x0-x1) < math::abs(y0-y1))
		{
			i16 t = x0;
			x0 = y0;
			y0 = t;
			t = x1;
			x1 = y1;
			y1 = t;
			step = true;
		}
		if (x0 > x1)
		{
			i16 t = x0;
			x0 = y0;
			y0 = t;
			t = x1;
			x1 = y1;
			y1 = t;
		}
		i16 dx = x1 - x0;
		i16 dy = y1 - y0;
		i16 derror2 = math::abs(dy) * 2;
		i16 error2 = 0;
		i16 y = y0;
		for (i16 i = x0; i <= x1; i++)
		{
			(step) ? drawPixel(y, i, r, g, b) : drawPixel(i, y, r, g, b);
			error2 += derror2;
			if (error2 > dx)
			{
				y += (y1 > y0 ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}
	void drawRectangle(u16 x, u16 y, u16 width, u16 height, u8 r, u8 g, u8 b)
	{
		const u8 bpp = vesaMode.bpp >> 3;
		u32 offset = y * vesaMode.pitch + x * bpp;
		for (u16 i = 0; i < height; i++)
		{
			for (u16 j = 0; j < width; j++)
			{
				offset += bpp;
				reinterpret_cast<u8*>(vesaMode.framebuffer)[offset + 0] = b;
				reinterpret_cast<u8*>(vesaMode.framebuffer)[offset + 1] = g;
				reinterpret_cast<u8*>(vesaMode.framebuffer)[offset + 2] = r;
			}
			offset += vesaMode.pitch - width * bpp;
		} 
	}
}