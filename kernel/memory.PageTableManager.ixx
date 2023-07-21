export module memory.PageTableManager;
import types;
import memory.utils;
import memory.PageIndex;
import memory.allocator;
import sl.Flags;
import sl.compare;

export namespace memory
{
#pragma pack(1)
	enum class MemoryFlagsBits : u8
	{
		ePRESENT = 1,
		eREAD_WRITE = 2,
		eUSER_SUPERVISOR = 4,
		eWRITE_THROUGH = 8,
		eCACHE_DISABLE = 16
	};
	using MemoryFlags = Flags<MemoryFlagsBits>;
	template <>
	struct FlagTraits<MemoryFlagsBits>
	{
		static constexpr bool isBitmask = true;
		static constexpr MemoryFlags allFlags =
			MemoryFlagsBits::ePRESENT |
			MemoryFlagsBits::eREAD_WRITE |
			MemoryFlagsBits::eUSER_SUPERVISOR |
			MemoryFlagsBits::eWRITE_THROUGH |
			MemoryFlagsBits::eCACHE_DISABLE;
	};
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
		void mapMemory(const void* physicalMemory, const void* virtualMemory, MemoryFlags flags = MemoryFlagsBits::ePRESENT | MemoryFlagsBits::eREAD_WRITE)
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