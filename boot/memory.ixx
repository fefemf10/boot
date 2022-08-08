export module memory;
import types;
import console;
export import :allocator;
export import :utils;
export namespace memory
{
	struct SMAP
	{
		u64* base;
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
			console::printf("Region: %x ", i);
			console::printf("base: %llx ", smap[i].base);
			console::printf("length: %llx ", smap[i].length);
			console::printf("type: %x ", smap[i].type);
			console::printf("acpi: %x ", smap[i].acpi);
			switch (smap[i].type)
			{
			case 1: console::puts(" (Available)\n"); break;
			case 2: console::puts(" (Reserved)\n"); break;
			case 3: console::puts(" (ACPI Reclaim)\n"); break;
			case 4: console::puts(" (ACPI NVS Memory)\n"); break;
			default: console::puts(" (Reserved)\n"); break;
			}
		}
		console::printf("Total RAM: %llx\n", reinterpret_cast<u64>(smap[size - 1].base) + smap[size - 1].length - 1);
		console::printf("Total 4KB blocks: %x\n", allocator::maxBlocks);
		console::printf("Used blocks: %x\n", allocator::usedBlocks);
		console::printf("Free blocks: %x\n", allocator::maxBlocks - allocator::usedBlocks);
	}
#pragma pack(1)
	struct PageDirectoryEntry
	{
		u64 present : 1;
		u64 readWrite : 1;
		u64 userSupervisor : 1;
		u64 writeThrough : 1;
		u64 cacheDisable : 1;
		u64 accessed : 1;
		u64 dirty : 1;
		u64 pageAttributeTable : 1;
		u64 global : 1;
		u64 available : 3;
		u64 address : 52;
	};
	__declspec(align(0x1000)) struct PageTable
	{
		PageDirectoryEntry entries[512];
	};
	void initialize()
	{
		u32 size = *reinterpret_cast<u32*>(0x27A00);
		SMAP* smap = reinterpret_cast<SMAP*>(0x27A04);
		u64 totalRAM = reinterpret_cast<u64>(smap[size - 1].base) + smap[size - 1].length - 1ull;
		allocator::initialize(reinterpret_cast<u64*>(0x30000), totalRAM);
		for (u32 i = 0; i < size; ++i)
		{
			if (smap[i].type == 1)
			{
				allocator::setRegion(smap[i].base, smap[i].length);
			}
		}
		//Reverse memory for this allocator map
		allocator::unsetRegion(reinterpret_cast<u64*>(0x30000), allocator::maxBlocks >> allocator::BLOCKSPERBYTE);
		allocator::unsetRegion(reinterpret_cast<u64*>(0), 0x7C00);
		allocator::unsetRegion(reinterpret_cast<u64*>(0x27A00), sizeof(size) + size * sizeof(SMAP));
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
		PageTable* PLM4;
		PageTableManager(PageTable* PLM4) : PLM4(PLM4)
		{

		}
		void mapMemory(void* virtualMemory, void* physicalMemory)
		{
			PageIndex index(reinterpret_cast<u64>(virtualMemory));
			PageDirectoryEntry PDE = PLM4->entries[index.pdp];
			PageTable* PDP;
			if (!PDE.present)
			{
				PDP = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
				set(PDP, 0, 0x1000);
				PDE.address = reinterpret_cast<u64>(PDP) >> 12;
				PDE.present = true;
				PDE.readWrite = true;
				PLM4->entries[index.pdp] = PDE;
			}
			else
			{
				PDP = reinterpret_cast<PageTable*>(PDE.address << 12);
			}
			
			PDE = PDP->entries[index.pd];
			PageTable* PD;
			if (!PDE.present)
			{
				PD = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
				set(PD, 0, 0x1000);
				PDE.address = reinterpret_cast<u64>(PD) >> 12;
				PDE.present = true;
				PDE.readWrite = true;
				PDP->entries[index.pd] = PDE;
			}
			else
			{
				PD = reinterpret_cast<PageTable*>(PDE.address << 12);
			}
			
			PDE = PD->entries[index.pt];
			PageTable* PT;
			if (!PDE.present)
			{
				PT = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
				set(PT, 0, 0x1000);
				PDE.address = reinterpret_cast<u64>(PT) >> 12;
				PDE.present = true;
				PDE.readWrite = true;
				PD->entries[index.pt] = PDE;
			}
			else
			{
				PT = reinterpret_cast<PageTable*>(PDE.address << 12);
			}

			PDE = PT->entries[index.p];
			PDE.address = reinterpret_cast<u64>(physicalMemory) >> 12;
			PDE.present = true;
			PDE.readWrite = true;
			PT->entries[index.p] = PDE;
		}
	};
}