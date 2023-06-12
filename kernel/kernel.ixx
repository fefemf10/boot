export module kernel;
import types;
//import cpuio;
//import IDT;
//import ISR;
//import IRQ;
//import pci;
//import memory;
//import memory.allocator;
//import memory.SMAP;
//import console;
//import sl;
//import ACPI;
//import PIT;
//import VESA;
//import driver.AHCI.structures;
//import glm;
//import glm.vec2;
//import glm.vec3;
//import glm.vec4;
//import glm.mat2x2;
//import glm.mat4x4;
//import glm.transform;
//import sl.math;
//import sl.numbers;
//import sl.vector;
//import SMBIOS;
//import memory.utils;
//import translator;

//struct Framebuffer
//{
//	void* baseAddress;
//	size_t bufferSize;
//	u32 width;
//	u32 height;
//	u32 pixelsPerScanline;
//};
//struct PSF1Header
//{
//	u8 magic[2];
//	u8 mode;
//	u8 charSize;
//};
//struct PSF1Font
//{
//	PSF1Header* psf1Header;
//	void* glyphBuffer;
//};
//struct MemoryDescriptor
//{
//	u32 type;
//	u32 pad;
//	void* physicalStart;
//	void* virtualStart;
//	u64 numberOfPages;
//	u64 attribute;
//};
//struct BootInfo
//{
//	Framebuffer* fb;
//	PSF1Font* font;
//	MemoryDescriptor* map;
//	u64 mapSize;
//	u64 mapDescriptorSize;
//};
//
//struct Point
//{
//	u32 x;
//	u32 y;
//} cursorPos{};
//void putChar(Framebuffer* fb, PSF1Font* font, u32 color, char8_t c, u32 offsetX, u32 offsetY)
//{
//	u32* pixels = (u32*)fb->baseAddress;
//	char8_t* fontPtr = reinterpret_cast<char8_t*>(font->glyphBuffer) + (c * font->psf1Header->charSize);
//	for (size_t y = offsetY; y < offsetY + 16; y++)
//	{
//		for (size_t x = offsetX; x < offsetX + 8; x++)
//		{
//			if ((*fontPtr & (0b10000000 >> (x - offsetX))) > 0)
//			{
//				*(u32*)(pixels + x + (y * fb->pixelsPerScanline)) = color;
//			}
//		}
//		++fontPtr;
//	}
//}
//
//void print(Framebuffer* fb, PSF1Font* font, const char8_t* str)
//{
//	const char8_t* c = str;
//	while (*c)
//	{
//		putChar(fb, font, 0x00FFFFFF, *c, cursorPos.x, cursorPos.y);
//		cursorPos.x += 8;
//		if (cursorPos.x + 8 > fb->width)
//		{
//			cursorPos.x = 0;
//			cursorPos.y += 16;
//		}
//		++c;
//	}
//}
int b = 1;
int mainCRTStartup()
{
	return f(0) + b;
}