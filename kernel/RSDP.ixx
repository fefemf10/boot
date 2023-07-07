export module RSDP;
import types;
import XSDT;
export namespace ACPI
{
#pragma pack(1)
	struct RSDP
	{
		bool isValid() const
		{
			u8 sum1{};
			u8 sum2{};
			for (size_t i = 0; i < 20; i++)
			{
				sum1 += reinterpret_cast<u8*>(const_cast<RSDP*>(this))[i];
			}
			for (size_t i = 20; i < sizeof(RSDP); i++)
			{
				sum2 += reinterpret_cast<u8*>(const_cast<RSDP*>(this))[i];
			}
			return !((sum1 & 0xFF) || (sum2 & 0xFF));
		}
		u8 signature[8];
		u8 checksum;
		u8 OEDId[6];
		u8 revision;
		u32 RSDTAddress;
		u32 length;
		XSDT& XSDT;
		u8 extendedChecksum;
		u8 rsv[3];
	};
}