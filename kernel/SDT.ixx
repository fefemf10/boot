export module SDT;
import types;
export namespace ACPI
{
	struct SDT
	{
		char signature[4];
		u32 length;
		u8 revision;
		u8 checksum;
		u8 OEMId[6];
		u8 OEMTableID[8];
		u32 OEMRevision;
		u32 creatorId;
		u32 creatorRevision;
		constexpr bool isValid() const
		{
			u8 sum{};
			for (size_t i = 0; i < length; i++)
				sum += reinterpret_cast<const u8*>(this)[i];
			return sum == 0;
		}
	};
}