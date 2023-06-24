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
	} *framebuffer;
	struct CursorPos
	{
		u32 x;
		u32 y;
	} cursorPos{};
	void nextPositionCursor()
	{
		cursorPos.x += 4;
		if (cursorPos.x + 4 > framebuffer->width)
		{
			cursorPos.x = 0;
			cursorPos.y += 8;
		}
	}
	void printRect(u32 color)
	{
		u32* pixels = (u32*)framebuffer->baseAddress;
		for (size_t y = cursorPos.y; y < cursorPos.y + 8; y++)
		{
			for (size_t x = cursorPos.x; x < cursorPos.x + 4; x++)
			{
				*(u32*)(pixels + x + (y * framebuffer->pixelsPerScanline)) = color;
			}
		}
		nextPositionCursor();
	}
	void printRects(u32 color, u32 count)
	{
		for (u32 i = 0; i < count; i++)
		{
			printRect(color);
		}
	}
}