export module RSDP;
import types;
import XSDT;
import console;
export namespace ACPI
{
#pragma pack(4)
	struct RSDP
	{
		u8 signature[8];
		u8 checksum;
		u8 OEDId[6];
		u8 revision;
		u32 RSDTAddress;
		u32 length;
		XSDT& XSDT;
		u8 extendedChecksum;
		u8 rsv[3];
		constexpr bool isValid() const
		{
			u8 sum1 = 0;
			u8 sum2 = 0;
			for (size_t i = 0; i < 20; i++)
			{
				sum1 += reinterpret_cast<const u8*>(this)[i];
			}
			for (size_t i = 0; i < sizeof(RSDP); i++)
			{
				sum2 += reinterpret_cast<const u8*>(this)[i];
			}
			return sum1 == 0 && sum2 == 0;
		}
	};
}