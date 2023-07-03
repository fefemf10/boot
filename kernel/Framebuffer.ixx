export module Framebuffer;
import types;
export
{
	enum class GraphicsPixelFormat : u32
	{
		PIXEL_RED_GREEN_BLUE_RESERVED_8BIT_PER_COLOR,
		PIXEL_BLUE_GREEN_RED_RESERVED_8BIT_PER_COLOR,
		PIXEL_BIT_MASK,
		PIXEL_BLT_ONLY,
		PIXEL_FORMAT_MAX
	};
	struct Framebuffer
	{
		void* baseAddress;
		size_t bufferSize;
		u32 width;
		u32 height;
		u32 pixelsPerScanline;
		GraphicsPixelFormat format;
		void clear(u32 color)
		{
			for (u32 i = 0; i < height; ++i)
			{
				const u64 pixels = reinterpret_cast<u64>(baseAddress) + (i * pixelsPerScanline * 4);
				for (u32 j = 0; j < width; j++)
				{
					reinterpret_cast<u32*>(pixels)[j] = color;
				}
			}
		}
		void clearLine(i64 line, u32 color)
		{
			const u64 pixels = reinterpret_cast<u64>(baseAddress) + (line * pixelsPerScanline * 4);
			for (u32 i = 0; i < width; i++)
			{
				reinterpret_cast<u32*>(pixels)[i] = color;
			}
		}
		void clearPixel(i64 x, i64 y, u32 color)
		{
			const u64 pixels = reinterpret_cast<u64>(baseAddress) + (y * pixelsPerScanline * 4);
			reinterpret_cast<u32*>(pixels)[x] = color;
		}
		void drawRectangle(i64 x, i64 y, i64 width, i64 height, u32 color)
		{
			for (u32 i = y; i < y + height; ++i)
			{
				const u64 pixels = reinterpret_cast<u64>(baseAddress) + (i * pixelsPerScanline * 4);
				for (u32 j = x; j < x + width; j++)
				{
					reinterpret_cast<u32*>(pixels)[j] = color;
				}
			}
		}
	} framebuffer;
}