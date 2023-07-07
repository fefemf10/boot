export module SDT;
import types;
import memory.PageTableManager;
export namespace ACPI
{
	struct SDT
	{
		static SDT* find(SDT* rsdt, const char8_t* signature)
		{
			memory::pageTableManager.mapMemory(rsdt, rsdt); //Important
			size_t entries = (rsdt->length - sizeof(SDT)) / 4;
			u32* ptrToSDT = reinterpret_cast<u32*>(reinterpret_cast<u64>(rsdt) + sizeof(SDT));
			for (size_t i = 0; i < entries; i++)
			{
				SDT* SDT = reinterpret_cast<ACPI::SDT*>(ptrToSDT[i]);
				if (SDT)
				{
					memory::pageTableManager.mapMemory(SDT->signature, SDT->signature); //Important
					for (size_t j = 0; j < 4; j++)
					{
						if (SDT->signature[j] != signature[j])
							break;
						if (i == 3)
							return SDT;
					}
				}
			}
			return nullptr;
		}
		constexpr bool isValid() const
		{
			u8 sum{};
			for (size_t i = 0; i < length; i++)
				sum += reinterpret_cast<u8*>(const_cast<SDT*>(this))[i];
			return sum == 0;
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
}