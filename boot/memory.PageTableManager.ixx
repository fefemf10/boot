export module memory.PageTableManager;
import types;
import memory.utils;
import memory.PageIndex;
import memory.allocator;
export namespace memory
{
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

	struct PageTableManager
	{
		PageTable* PLM4;
		PageTableManager() = default;
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