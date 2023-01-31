export module VESA;
import VESA.structures;
import types;
import memory;
import memory.allocator;
import console;
import sl.math;
export namespace VESA
{
	VESAInfo* vesaInfo;
	VESAModeInfo* vesaModesInfo;
	VESAModeInfo vesaMode;
	u32 countModes;
	u16 currentMode;
	u32* fb1;
	u32* fb2;
	u32* cfb;
	u32* bfb;
	bool fb{false};
	void initialize()
	{
		vesaInfo = reinterpret_cast<VESAInfo*>(VESAInfo::address);
		countModes = *reinterpret_cast<u32*>(VESAModeInfo::address);
		currentMode = *reinterpret_cast<u16*>(VESAModeInfo::address + 4);
		vesaModesInfo = reinterpret_cast<VESAModeInfo*>(VESAModeInfo::address + 6);
		memory::pageTableManager.mapMemory(vesaModesInfo, vesaModesInfo);
		vesaMode = vesaModesInfo[currentMode];
		i32 size = vesaMode.height * vesaMode.pitch;
		fb1 = reinterpret_cast<u32*>(memory::allocator::allocBlocks(((size - 1) >> 12) + 1));
		fb2 = reinterpret_cast<u32*>(memory::allocator::allocBlocks(((size - 1) >> 12) + 1));
		cfb = fb1;
		bfb = fb2;
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(fb1), reinterpret_cast<void*>(fb1), size);
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(fb2), reinterpret_cast<void*>(fb2), size);
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(vesaMode.framebuffer), reinterpret_cast<void*>(vesaMode.framebuffer), size);
	}
	void copy()
	{
		const u8 bpp = vesaMode.bpp >> 3;
		u32 offset = 0;
		for (u16 i = 0; i < vesaMode.height; i++)
		{
			for (u16 j = 0; j < vesaMode.width; j++)
			{
				reinterpret_cast<u32*>(vesaMode.framebuffer)[offset] = reinterpret_cast<u32*>(bfb)[offset];
				offset++;
			}
			offset += (vesaMode.pitch >> 2) - vesaMode.width;
		}
	}
	void swap()
	{
		if (fb)
		{
			cfb = fb2;
			bfb = fb1;
		}
		else
		{
			cfb = fb1;
			bfb = fb2;
		}
		copy();
	}
	void clear()
	{
		const u8 bpp = vesaMode.bpp >> 3;
		u32 offset = 0;
		for (u16 i = 0; i < vesaMode.height; i++)
		{
			for (u16 j = 0; j < vesaMode.width; j++)
			{
				reinterpret_cast<u32*>(bfb)[offset] = 0;
				offset++;
			}
			offset += (vesaMode.pitch >> 2) - vesaMode.width;
		}
	}
	void drawPixel(u16 x, u16 y, u32 argb)
	{
		u32 pixel_offset = (y * vesaMode.pitch + x * 4) >> 2;
		reinterpret_cast<u32*>(bfb)[pixel_offset] = argb;
	}
	void drawLine(i32 x0, i32 y0, i32 x1, i32 y1, u32 argb)
	{
		bool step{};
		if (std::abs(x0-x1) < std::abs(y0-y1))
		{
			i32 t = x0;
			x0 = y0;
			y0 = t;
			t = x1;
			x1 = y1;
			y1 = t;
			step = true;
		}
		if (x0 > x1)
		{
			i32 t = x0;
			x0 = x1;
			x1 = t;
			t = y0;
			y0 = y1;
			y1 = t;
		}
		i32 dx = x1 - x0;
		i32 dy = y1 - y0;
		i32 derror2 = std::abs(dy) * 2;
		i32 error2 = 0;
		i32 y = y0;
		for (i32 i = x0; i <= x1; i++)
		{
			(step) ? drawPixel(y, i, argb) : drawPixel(i, y, argb);
			error2 += derror2;
			if (error2 > dx)
			{
				y += (y1 > y0 ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}
	void drawRectangle(u16 x, u16 y, u16 width, u16 height, u32 argb)
	{
		const u8 bpp = vesaMode.bpp >> 3;
		u32 offset = y * vesaMode.width + x;
		for (u16 i = 0; i < height; i++)
		{
			for (u16 j = 0; j < width; j++)
			{
				reinterpret_cast<u32*>(bfb)[offset] = argb;
				offset++;
			}
			offset += (vesaMode.pitch >> 2) - width;
		}
	}
}