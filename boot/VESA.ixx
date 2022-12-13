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
		u16 attributes;
		u16 window;
		u16 granularity;
		u16 window_size;
		u16 segmenta;
		u16 segmentb;
		u32 winfunc;
		u16 pitch;
		u16 width;
		u16 height;
		u8 wchar;
		u8 ychar;
		u8 planes;
		u8 bpp;
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
	void initialize()
	{
		vesaInfo = reinterpret_cast<VESAInfo*>(VESAInfo::address);
		vesaModesInfo = reinterpret_cast<VESAModeInfo*>(VESAModeInfo::address);
	}
}