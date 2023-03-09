export module VESA;
import VESA.structures;
import types;
import memory;
import memory.allocator;
import memory.utils;
import console;
import sl.math;
import sl.algorithm;
export namespace VESA
{
	VESAInfo* vesaInfo;
	VESAModeInfo* vesaModesInfo;
	VESAModeInfo vesaMode;
	u32 countModes;
	u16 currentMode;
	u32* fb1;
	u32* bfb;
	u32 sizeInDW;
	bool fb{ true };
	void initialize()
	{
		vesaInfo = reinterpret_cast<VESAInfo*>(VESAInfo::address);
		countModes = *reinterpret_cast<u32*>(VESAModeInfo::address);
		currentMode = *reinterpret_cast<u16*>(VESAModeInfo::address + 4);
		vesaModesInfo = reinterpret_cast<VESAModeInfo*>(VESAModeInfo::address + 6);
		memory::pageTableManager.mapMemory(vesaModesInfo, vesaModesInfo);
		vesaMode = vesaModesInfo[currentMode];
		i32 size = vesaMode.height * vesaMode.pitch;
		sizeInDW = (vesaMode.pitch * vesaMode.height) >> 2;
		fb1 = reinterpret_cast<u32*>(memory::allocator::allocBlocks(((size - 1) >> 12) + 1));;
		bfb = fb1;
		memory::pageTableManager.mapMemory(fb1, fb1, size);
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(vesaMode.framebuffer), reinterpret_cast<void*>(vesaMode.framebuffer), size);
	}
	void copy()
	{
		/*u32 offset = 0;
		const u32 m = (vesaMode.pitch >> 2) - vesaMode.width;
		for (i32 i = 0; i < vesaMode.height; ++i, offset += m)
			for (i32 j = 0; j < vesaMode.width; ++j, ++offset)
				reinterpret_cast<u32*>(vesaMode.framebuffer)[offset] = bfb[offset];*/
		//memory::move(reinterpret_cast<u32*>(vesaMode.framebuffer), bfb, vesaMode.height * vesaMode.pitch);
		u32 offset = 0;
		const u32 m = (vesaMode.pitch >> 2) - vesaMode.width;
		for (i32 i = 0; i < vesaMode.height; ++i)
		{
			memory::move(reinterpret_cast<u32*>(vesaMode.framebuffer) + offset, bfb + offset, vesaMode.pitch);
			offset += m + vesaMode.width;
		}
		//memory::move(reinterpret_cast<u32*>(vesaMode.framebuffer) + offset, bfb + offset, vesaMode.height * vesaMode.pitch);
	}
	void swap()
	{
		copy();
	}
	void clear()
	{
		//memory::set(reinterpret_cast<u32*>(vesaMode.framebuffer), 0xff, vesaMode.height * vesaMode.pitch);
		u32 offset = 0;
		const u32 m = (vesaMode.pitch >> 2) - vesaMode.width;
		for (i32 i = 0; i < vesaMode.height; ++i)
		{
			memory::set(bfb + offset, 0xFF, vesaMode.pitch);
			offset += m + vesaMode.width;
			/*for (i32 j = 0; j < vesaMode.width; ++j, ++offset)
				bfb[offset] = 0;*/
		}
	}
	inline void drawPixel(u32 offset, u32 argb)
	{
		bfb[offset] = argb;
	}
	inline void drawPixel(i32 x, i32 y, u32 argb)
	{
		u32 pixel_offset = (y * vesaMode.pitch + x * 4) >> 2;
		bfb[pixel_offset] = argb;
	}
	void drawLine(i32 x0, i32 y0, i32 x1, i32 y1, u32 argb)
	{
		bool step{};
		if (std::abs(x0 - x1) < std::abs(y0 - y1))
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
		const i32 dx = x1 - x0;
		const i32 dy = y1 - y0;
		const i32 derror2 = std::abs(dy) << 1;
		i32 error2 = 0;
		i32 y = y0;
		i32 ypitch = y * vesaMode.pitch;
		for (i32 i = x0; i <= x1; i++)
		{
			(step) ? drawPixel(y, i, argb) : drawPixel(i, y, argb);
			error2 += derror2;
			if (error2 > dx)
			{
				y += (y1 > y0 ? 1 : -1);
				ypitch = y * vesaMode.pitch;
				error2 -= dx << 1;
			}
		}
	}
	void drawRectangle(i32 x, i32 y, u16 width, u16 height, u32 argb)
	{
		u32 offset = y * vesaMode.width + x;
		const u32 m = (vesaMode.pitch >> 2) - width;
		for (u32 i = 0; i < height; ++i, offset += m)
			for (u32 j = 0; j < width; ++j, ++offset)
				bfb[offset] = argb;
	}
	void drawCircle(i32 x, i32 y, i32 r, u32 argb)
	{
		int dx = 0;
		int dy = r;
		int delta = 1 - 2 * r;
		int error = 0;
		while (dy >= dx)
		{
			drawPixel(x + dx, y + dy, argb);
			drawPixel(x + dx, y - dy, argb);
			drawPixel(x - dx, y + dy, argb);
			drawPixel(x - dx, y - dy, argb);
			drawPixel(x + dy, y + dx, argb);
			drawPixel(x + dy, y - dx, argb);
			drawPixel(x - dy, y + dx, argb);
			drawPixel(x - dy, y - dx, argb);
			error = 2 * (delta + dy) - 1;
			if ((delta < 0) && (error <= 0))
			{
				delta += 2 * ++dx + 1;
				continue;
			}
			if ((delta > 0) && (error > 0))
			{
				delta -= 2 * --dy + 1;
				continue;
			}
			delta += 2 * (++dx - --dy);
		}
	}
}