export module memory;
import types;
import teletype;
export namespace memory
{

	struct SMAP
	{
		u64 base;
		u64 length;
		u32 type;
		u32 acpi;
	};
	void printSMAP()
	{
		u32 size = *reinterpret_cast<u32*>(0x27A00);
		SMAP* smap = reinterpret_cast<SMAP*>(0x27A04);
		for (u64 i = 0; i < size; i++)
		{
			teletype::printf("Region: %x ", i);
			teletype::printf("base: %llx ", smap[i].base);
			teletype::printf("length: %llx ", smap[i].length);
			teletype::printf("type: %x ", smap[i].type);
			teletype::printf("acpi: %x ", smap[i].acpi);
			switch (smap[i].type)
			{
			case 1: teletype::puts(" (Available)\n\r"); break;
			case 2: teletype::puts(" (Reserved)\n\r"); break;
			case 3: teletype::puts(" (ACPI Reclaim)\n\r"); break;
			case 4: teletype::puts(" (ACPI NVS Memory)\n\r"); break;
			default: teletype::puts(" (Reserved)\n\r"); break;
			}
		}
		teletype::printf("Total RAM: %llx\n", smap[size - 1].base + smap[size - 1].length - 1);
	}
#pragma pack(1)
	struct PageDirectoryEntry
	{
		bool present : 1;
		bool readWrite : 1;
		bool userSupervisor : 1;
		bool writeThrough : 1;
		bool cacheDisable : 1;
		bool accessed : 1;
		bool dirty : 1;
		bool pageAttributeTable : 1;
		bool global : 1;
		u8 available : 3;
		u64 address : 52;
	};
	__declspec(align(0x1000)) struct PageTable
	{
		PageDirectoryEntry entries[512];
	};
	void initialize()
	{
		//plm[0]-512GB
		//pdp[0]-1GB
		//pd[0]-2MB
		//pt[0]-4KB
	}
	void* set(void* p, u8 value, u64 size)
	{
		for (u64 i = 0; i < size; ++i)
		{
			*reinterpret_cast<u8*>(reinterpret_cast<u64>(p) + i) = value;
		}
		return p;
	}
	void copy(void* source, void* destination, u64 size)
	{
		for (u64 i = 0; i < size; ++i)
		{
			reinterpret_cast<u8*>(destination)[i] = reinterpret_cast<u8*>(source)[i];
		}
	}
	void* alloc(u64 size)
	{
		return nullptr;
	}
	void free(void* p)
	{

	}
	struct PageIndex
	{
		PageIndex(u64 virtualAddress)
		{
			virtualAddress >>= 12;
			p = virtualAddress & 0x1FF;
			virtualAddress >>= 9;
			pt = virtualAddress & 0x1FF;
			virtualAddress >>= 9;
			pd = virtualAddress & 0x1FF;
			virtualAddress >>= 9;
			pdp = virtualAddress & 0x1FF;
		}
		u64 pdp;
		u64 pd;
		u64 pt;
		u64 p;
	};
	struct PageTableManager
	{
		PageTableManager(PageTable* PLM4) : PLM4(PLM4)
		{

		}
		void mapMemory(void* virtualMemory, void* physicalMemory)
		{
			//PageIndex index(reinterpret_cast<u64>(virtualMemory));
			//PageDirectoryEntry PDE{};
			//PDE = PLM4->entries[index.pdp];
			//PageTable* PDP;
			//if (!PDE.present)
			//{
			//	//PDP = reinterpret_cast<PageTable*>(getPage());
			//	set(PDP, 0, 0x1000);
			//	PDE.address = reinterpret_cast<u64>(PDP) >> 12;
			//	PDE.present = true;
			//	PDE.readWrite = true;
			//	PLM4->entries[index.pdp] = PDE;
			//}
			//else
			//{
			//	PDP = reinterpret_cast<PageTable*>(PDE.address << 12);
			//}

			//PDE = PDP->entries[index.pd];
			//PageTable* PD;
			//if (!PDE.present)
			//{
			//	//PD = reinterpret_cast<PageTable*>(getPage());
			//	set(PD, 0, 0x1000);
			//	PDE.address = reinterpret_cast<u64>(PD) >> 12;
			//	PDE.present = true;
			//	PDE.readWrite = true;
			//	PLM4->entries[index.pd] = PDE;
			//}
			//else
			//{
			//	PD = reinterpret_cast<PageTable*>(PDE.address << 12);
			//}

			//PDE = PDP->entries[index.pt];
			//PageTable* PT;
			//if (!PDE.present)
			//{
			//	//PT = reinterpret_cast<PageTable*>(getPage());
			//	set(PT, 0, 0x1000);
			//	PDE.address = reinterpret_cast<u64>(PT) >> 12;
			//	PDE.present = true;
			//	PDE.readWrite = true;
			//	PLM4->entries[index.pt] = PDE;
			//}
			//else
			//{
			//	PT = reinterpret_cast<PageTable*>(PDE.address << 12);
			//}

			//PDE = PT->entries[index.p];
			//PDE.address = reinterpret_cast<u64>(PT) >> 12;
			//PDE.present = true;
			//PDE.readWrite = true;
			//PT->entries[index.p] = PDE;
		}
		PageTable* PLM4;
	};
}