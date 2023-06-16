export module Framebuffer;
import types;
export
{
	struct Framebuffer
	{
		void* baseAddress;
		size_t bufferSize;
		u32 width;
		u32 height;
		u32 pixelsPerScanline;
	} *framebuffer;
	struct CursorPos
	{
		u32 x;
		u32 y;
	} cursorPos{};
}