export module GDT;
import types;
export namespace GDT
{
	constexpr u64 SEG_SAVL(u64 x)
	{
		return (x) << 0x0C;
	}

	constexpr u64 SEG_LONG(u64 x)
	{
		return x << 0x0D;
	}

	constexpr u64 SEG_DB(u64 x)
	{
		return x << 0x0E;
	}

	constexpr u64 SEG_GRAN(u64 x)
	{
		return (x) << 0x0F;
	}

	constexpr u64 SEG_DESCTYPE(u64 x)
	{
		return (x) << 0x04;
	}

	constexpr u64 SEG_DPL(u64 x)
	{
		return (x & 0x03) << 0x05;
	}

	constexpr u64 SEG_P(u64 x)
	{
		return x << 0x07;
	}

	constexpr u64 SEG_DATA_RD = 0x00;
	constexpr u64 SEG_DATA_RDA = 0x01;
	constexpr u64 SEG_DATA_RDWR = 0x02;
	constexpr u64 SEG_DATA_RDWRA = 0x03;
	constexpr u64 SEG_DATA_RDEXPD = 0x04;
	constexpr u64 SEG_DATA_RDEXPDA = 0x05;
	constexpr u64 SEG_DATA_RDWREXPD = 0x06;
	constexpr u64 SEG_DATA_RDWREXPDA = 0x07;
	constexpr u64 SEG_CODE_EX = 0x08;
	constexpr u64 SEG_CODE_EXA = 0x09;
	constexpr u64 SEG_CODE_EXRD = 0x0A;
	constexpr u64 SEG_CODE_EXRDA = 0x0B;
	constexpr u64 SEG_CODE_EXC = 0x0C;
	constexpr u64 SEG_CODE_EXCA = 0x0D;
	constexpr u64 SEG_CODE_EXRDC = 0x0E;
	constexpr u64 SEG_CODE_EXRDCA = 0x0F;

	constexpr u64 GDT_CODE_PL0()
	{
		return SEG_P(1) | SEG_DPL(0) | SEG_DESCTYPE(1) | SEG_SAVL(0) | SEG_LONG(1) | SEG_DB(0) | SEG_GRAN(1) | SEG_CODE_EXRD;
	}

	constexpr u64 GDT_DATA_PL0()
	{
		return SEG_P(1) | SEG_DPL(0) | SEG_DESCTYPE(1) | SEG_SAVL(0) | SEG_LONG(1) | SEG_DB(0) | SEG_GRAN(1) | SEG_DATA_RDWR;
	}

	constexpr u64 GDT_CODE_PL3()
	{
		return SEG_P(1) | SEG_DPL(3) | SEG_DESCTYPE(1) | SEG_SAVL(0) | SEG_LONG(1) | SEG_DB(0) | SEG_GRAN(1) | SEG_CODE_EXRD;
	}

	constexpr u64 GDT_DATA_PL3()
	{
		return SEG_P(1) | SEG_DPL(3) | SEG_DESCTYPE(1) | SEG_SAVL(0) | SEG_LONG(1) | SEG_DB(0) | SEG_GRAN(1) | SEG_DATA_RDWR;
	}

	void createDescriptor(u16 flag)
	{
		u64 descriptor{};
		descriptor |= (flag << 8) & 0x00F0FF00;
		descriptor <<= 32;
	}

	struct GDTEntry
	{
		u16 limit0;
		u16 base0;
		u8 base1;
		u8 access;
		u8 limit1;
		u8 base2;
	};

	struct GDT
	{
		GDTEntry null;
		GDTEntry kernelCode;
		GDTEntry kernelData;
		GDTEntry userNull;
		GDTEntry userCode;
		GDTEntry userData;
	};

	constinit _declspec(align(0x1000)) GDT DefaultGDT =
	{
		{0, 0, 0, 0x00, 0x00, 0},
		{0, 0, 0, 0x9A, 0xA0, 0},
		{0, 0, 0, 0x92, 0xA0, 0},
		{0, 0, 0, 0x00, 0x00, 0},
		{0, 0, 0, 0x9A, 0xA0, 0},
		{0, 0, 0, 0x92, 0xA0, 0},
	};

#pragma pack(1)
	struct GDTDescriptor
	{
		u16 size;
		u64 offset;
	} constinit gdtDescriptor{ sizeof(GDT) - 1, reinterpret_cast<u64>(&DefaultGDT) };
}