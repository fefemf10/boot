export module memory.PageTableManager;
import types;
import memory.PageTable;
import memory.utils;
import memory.PageIndex;
import memory.allocator;
import memory.flags;

export namespace memory
{
	struct PageTableManager
	{
		PageTable* plm4;
		PageTableManager() = default;
		PageTableManager(PageTable* PLM4) : plm4(PLM4)
		{

		}
		void fixedMapMemory(u64 allocStart, const void* physicalMemory, const void* virtualMemory, MemoryFlagsBits flags = MemoryFlagsBits(MemoryFlagsBits::ePRESENT | MemoryFlagsBits::eREAD_WRITE))
		{
			PageIndex index(reinterpret_cast<u64>(virtualMemory));
			PageDirectoryEntry PDE = plm4->entries[index.pdp];
			PageTable* PDP;
			if (!PDE.present)
			{
				PDP = reinterpret_cast<PageTable*>(allocStart);
				set(PDP, 0, 0x1000);
				PDE.address = reinterpret_cast<u64>(PDP) >> 12;
				PDE.present = true;
				PDE.readWrite = true;
				plm4->entries[index.pdp] = PDE;
				allocStart += 0x1000;
			}
			else
			{
				PDP = reinterpret_cast<PageTable*>(PDE.address << 12);
			}
			PDE = PDP->entries[index.pd];
			PageTable* PD;
			if (!PDE.present)
			{
				PD = reinterpret_cast<PageTable*>(allocStart);
				set(PD, 0, 0x1000);
				PDE.address = reinterpret_cast<u64>(PD) >> 12;
				PDE.present = true;
				PDE.readWrite = true;
				PDP->entries[index.pd] = PDE;
				allocStart += 0x1000;
			}
			else
			{
				PD = reinterpret_cast<PageTable*>(PDE.address << 12);
			}
			PDE = PD->entries[index.pt];
			PageTable* PT;
			if (!PDE.present)
			{
				PT = reinterpret_cast<PageTable*>(allocStart);
				set(PT, 0, 0x1000);
				PDE.address = reinterpret_cast<u64>(PT) >> 12;
				PDE.present = true;
				PDE.readWrite = true;
				PD->entries[index.pt] = PDE;
				allocStart += 0x1000;
			}
			else
			{
				PT = reinterpret_cast<PageTable*>(PDE.address << 12);
			}
			PDE = PT->entries[index.p];
			PDE.address = reinterpret_cast<u64>(physicalMemory) >> 12;
			PDE.present = true;
			PDE.readWrite = true;
			PDE.present = static_cast<bool>(flags & MemoryFlagsBits::ePRESENT);
			PDE.readWrite = static_cast<bool>(flags & MemoryFlagsBits::eREAD_WRITE);
			PDE.cacheDisable = static_cast<bool>(flags & MemoryFlagsBits::eCACHE_DISABLE);
			PT->entries[index.p] = PDE;
		}
		void mapMemory(const void* physicalMemory, const void* virtualMemory, MemoryFlagsBits flags = MemoryFlagsBits(MemoryFlagsBits::ePRESENT | MemoryFlagsBits::eREAD_WRITE))
		{
			PageIndex index(reinterpret_cast<u64>(virtualMemory));
			PageDirectoryEntry PDE = plm4->entries[index.pdp];
			PageTable* PDP;
			if (!PDE.present)
			{
				PDP = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
				set(PDP, 0, 0x1000);
				PDE.address = reinterpret_cast<u64>(PDP) >> 12;
				PDE.present = true;
				PDE.readWrite = true;
				plm4->entries[index.pdp] = PDE;
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
			PDE.present = static_cast<bool>(flags & MemoryFlagsBits::ePRESENT);
			PDE.readWrite = static_cast<bool>(flags & MemoryFlagsBits::eREAD_WRITE);
			PDE.cacheDisable = static_cast<bool>(flags & MemoryFlagsBits::eCACHE_DISABLE);
			PT->entries[index.p] = PDE;
		}
		void mapMemory(const void* physicalMemory, const void* virtualMemory, u64 countPages)
		{
			u8* virt = reinterpret_cast<u8*>((u64)virtualMemory);
			u8* phys = reinterpret_cast<u8*>((u64)physicalMemory);
			for (size_t i = 0; i < countPages; ++i, virt += 0x1000, phys += 0x1000)
			{
				mapMemory(phys, virt);
			}
		}
	} pageTableManager;
}