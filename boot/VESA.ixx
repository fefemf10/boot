export module VESA;
import types;
export namespace VESA
{
#pragma pack(1)
	struct VESAInfo
	{
		char8_t signature[4];
		u16 version;
		u32 oem;
		u32 cap;
		u32 videoModes;
		u16 videoMemory;
		u16 softwareRevision;
		u32 vendor;
		u32 productName;
		u32 productRevision;
		u8 rsv[222];
		u8 oemData[256];
		constexpr static u64 address = 0x27A00;
	};
#pragma pack(1)
	struct VESAModeInfo
	{
		u16 attributes;//0
		u16 window;//2
		u16 granularity;//4
		u16 window_size;//6
		u32 segment;//8
		u32 winfunc;//12
		u16 pitch;//16
		u16 width;//18
		u16 height;//20
		u8 wchar;//22
		u8 ychar;//23
		u8 planes;//24
		u8 bpp;//25
		u8 banks;
		u8 memory_model;
		u8 bankSize;
		u8 imagePages;
		u8 rsv0;
		u8 redMask;
		u8 redPosition;
		u8 greenMask;
		u8 greenPosition;
		u8 blueMask;
		u8 bluePosition;
		u8 rsvMask;
		u8 rsvPosition;
		u8 directColorAttributes;
		u32 framebuffer;
		u32 offScreenMemoryOff;
		u16 offScreenMemorySize;
		u8 reserved1[206];
		constexpr static u64 address = 0x27C00;
	};
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
	}
	void drawPixel(u16 x, u16 y, u8 r, u8 g, u8 b)
	{
		u32 pixel_offset = y * vesaModesInfo[currentMode].pitch + (x * (vesaModesInfo[currentMode].bpp / 8)) + vesaModesInfo[currentMode].framebuffer;
		reinterpret_cast<u8*>(pixel_offset)[0] = r;
		reinterpret_cast<u8*>(pixel_offset)[1] = g;
		reinterpret_cast<u8*>(pixel_offset)[2] = b;
	}
}