export module ACPI;
import types;
export namespace ACPI
{
#pragma pack(1)
	struct RSDP
	{
		static RSDP* find()
		{
			const char8_t* rsdptr = u8"RSD PTR ";
			u64 ebda = static_cast<u64>(*reinterpret_cast<u16*>(0x40E)) << 4ull;
			for (size_t* i = reinterpret_cast<u64*>(ebda); i < reinterpret_cast<u64*>(ebda + 0x400); i += 2)
			{
				for (size_t j = 0; j < 8; j++)
				{
					if (reinterpret_cast<char8_t*>(i)[j] != rsdptr[j])
						break;
					if (j == 7)
						return reinterpret_cast<RSDP*>(i);
				}
			}
			for (size_t* i = reinterpret_cast<u64*>(0xE0000); i < reinterpret_cast<u64*>(0xFFFFF); i += 2)
			{
				for (size_t j = 0; j < 8; j++)
				{
					if (reinterpret_cast<char8_t*>(i)[j] != rsdptr[j])
						break;
					if (j == 7)
						return reinterpret_cast<RSDP*>(i);
				}
			}
			return nullptr;
		}
		bool isValid() const
		{
			u64 sum{};
			for (size_t i = 0; i < 20; i++)
			{
				sum += reinterpret_cast<u8*>(const_cast<RSDP*>(this))[i];
			}
			return !(sum & 0xFF);
		}
		u8 signature[8];
		u8 checksum;
		u8 OEDId[6];
		u8 revision;
		u32 RSDTAddress;
	};
#pragma pack(1)
	struct SDTHeader
	{
		static SDTHeader* find(ACPI::SDTHeader* rsdt, const char8_t* signature)
		{
			size_t entries = (rsdt->length - sizeof(ACPI::SDTHeader)) / 4;
			u32* ptrToSDT = reinterpret_cast<u32*>(reinterpret_cast<u64>(rsdt) + sizeof(ACPI::SDTHeader));
			for (size_t i = 0; i < entries; i++)
			{
				ACPI::SDTHeader* SDTHeader = reinterpret_cast<ACPI::SDTHeader*>(ptrToSDT[i]);
				for (size_t j = 0; j < 4; j++)
				{
					if (SDTHeader->signature[j] != signature[j])
						break;
					if (i == 3)
						return SDTHeader;
				}
			}
			return nullptr;
		}
		u8 signature[4];
		u32 length;
		u8 revision;
		u8 checksum;
		u8 OEMId[6];
		u8 OEMTableID[8];
		u32 OEMRevision;
		u32 creatorId;
		u32 creatorRevision;
	};
#pragma pack(1)
	struct MCFGHeader
	{
		SDTHeader header;
		u64 reserved;
	};
	struct DeviceConfig
	{
		u64 baseAddress;
		u16 pciSegmentGroup;
		u8 startBus;
		u8 endBus;
		u32 reserved;
	};
}