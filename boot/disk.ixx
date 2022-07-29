export module disk;
import types;
export namespace disk
{
	struct PRDT
	{
		u32 description;
		u32 commandStatus;
		u32 commandTableBaseAddress;
		u32 commandTableBaseAddressUpper;
		u32 reserved[4];
	};
	struct PRD
	{
		u32 dba;
		u32 dbau;
		u32 reserved;
		u32 description;
		constexpr u32 getDataBaseAddress()
		{
			return dba >> 1;
		}
		constexpr u32 getInterruptCompletion()
		{
			return description >> 31;
		}
		constexpr u32 getDataByteCount()
		{
			return description & 0x3FFFFF;
		}
	};
	constexpr u64 ATA0 = 0x01F0;
	constexpr u64 ATA1 = 0x0170;
	const u64* PRDT = reinterpret_cast<u64*>(0x8e00);
	void read()
	{

	}
	void write()
	{

	}
}