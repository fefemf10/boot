export module memory;
export import memory.utils;
export import :allocator;
import types;
import cpuio;
import console;
import teletype;

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
			console::printf(u8"Region: %x ", i);
			console::printf(u8"base: %llx ", smap[i].base);
			console::printf(u8"length: %llx ", smap[i].length);
			console::printf(u8"type: %x ", smap[i].type);
			console::printf(u8"acpi: %x ", smap[i].acpi);
			switch (smap[i].type)
			{
			case 1: console::puts(u8" (Available)\n"); break;
			case 2: console::puts(u8" (Reserved)\n"); break;
			case 3: console::puts(u8" (ACPI Reclaim)\n"); break;
			case 4: console::puts(u8" (ACPI NVS Memory)\n"); break;
			default: console::puts(u8" (Reserved)\n"); break;
			}
		}
		console::printf(u8"Total RAM: %llx\n", reinterpret_cast<u64>(smap[size - 1].base) + smap[size - 1].length - 1);
		console::printf(u8"Total 4KB blocks: %x\n", allocator::maxBlocks);
		console::printf(u8"Used blocks: %x\n", allocator::usedBlocks);
		console::printf(u8"Free blocks: %x\n", allocator::maxBlocks - allocator::usedBlocks);
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
	PageTable* PLM4;
	PageTableManager pageTableManager;
	extern "C" void loadGDT(PageTable * plm4);
	void initializeHeap(void* address, u64 size);
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
		PLM4 = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
		set(PLM4, 0, 0x1000);
		pageTableManager = PageTableManager(PLM4);
		for (u64 i = 0; i < 0x9000000/*128mb*/; i += 0x1000)
		{
			pageTableManager.mapMemory((void*)i, (void*)i);
		}
		loadGDT(PLM4);
		initializeHeap((void*)0x0000100000000000, 0x10);
	}
	struct HeapSegment;
	HeapSegment* lastHS;
#pragma pack(8)
	struct HeapSegment
	{
		u64 size;
		HeapSegment* next;
		HeapSegment* last;
		bool free;
		void combineForward()
		{
			if (!next) return;
			if (!next->free) return;
			if (next == lastHS) lastHS = this;
			if (next->next != nullptr)
				next->next->last = this;
			size = size + next->size + sizeof(HeapSegment);
		}
		void combineBackward()
		{
			if (last && last->free) last->combineForward();
		}
		HeapSegment* split(u64 size)
		{
			if (size < 0x10) return nullptr;
			i64 splitSize = this->size - size - sizeof(HeapSegment);
			if (splitSize < 0x10) return nullptr;
			HeapSegment* newSplit = reinterpret_cast<HeapSegment*>(reinterpret_cast<u64>(this) + size + sizeof(HeapSegment));
			next->last = newSplit;
			newSplit->next = next;
			next = newSplit;
			newSplit->last = this;
			newSplit->size = splitSize;
			newSplit->free = free;
			this->size = size;
			if (lastHS == this) lastHS = newSplit;
			return newSplit;
		}
	};
	void* heapStart;
	void* heapEnd;
	
	void initializeHeap(void* address, u64 size)
	{
		void* pos = address;
		PageTableManager pageTableManager(PLM4);
		for (size_t i = 0; i < size; ++i)
		{
			pageTableManager.mapMemory(pos, allocator::allocBlocks(1));
			pos = reinterpret_cast<void*>(reinterpret_cast<u64>(pos) + 0x1000);
		}
		u64 heapSize = size * 0x1000;
		heapStart = address;
		heapEnd = reinterpret_cast<void*>(reinterpret_cast<u64>(heapStart) + heapSize);
		HeapSegment* startSegment = reinterpret_cast<HeapSegment*>(address);
		startSegment->size = heapSize - sizeof(HeapSegment);
		startSegment->next = nullptr;
		startSegment->last = nullptr;
		startSegment->free = true;
		lastHS = startSegment;
	}
	void extendMap(u64 size)
	{
		if (size % 0x1000)
		{
			size -= size % 0x1000;
			size += 0x1000;
		}
		u64 pages = size / 0x1000;
		HeapSegment* newSegment = reinterpret_cast<HeapSegment*>(heapEnd);
		PageTableManager pageTableManager(PLM4);
		for (size_t i = 0; i < pages; ++i)
		{
			pageTableManager.mapMemory(heapEnd, allocator::allocBlocks(1));
			heapEnd = reinterpret_cast<void*>(reinterpret_cast<u64>(heapEnd) + 0x1000);
		}
		newSegment->free = true;
		newSegment->last = lastHS;
		lastHS->next = newSegment;
		lastHS = newSegment;
		newSegment->next = nullptr;
		newSegment->size = size - sizeof(HeapSegment);
		newSegment->combineBackward();
	}
	__declspec(allocator) void* alloc(u64 size)
	{
		if (size % 0x10 > 0)
		{
			size -= (size % 0x10);
			size += 0x10;
		}
		if (!size) return nullptr;
		HeapSegment* currentSegment = reinterpret_cast<HeapSegment*>(heapStart);
		while (true)
		{
			if (currentSegment->free)
			{
				if (currentSegment->size > size)
				{
					currentSegment->split(size);
					currentSegment->free = false;
					return reinterpret_cast<void*>(reinterpret_cast<u64>(currentSegment) + sizeof(HeapSegment));
				}
				if (currentSegment->size == size)
				{
					currentSegment->free = false;
					return reinterpret_cast<void*>(reinterpret_cast<u64>(currentSegment) + sizeof(HeapSegment));
				}
			}
			if (!currentSegment->next) break;
			currentSegment = currentSegment->next;
		}
		extendMap(size);
		return alloc(size);
	}
	void free(void* address)
	{
		HeapSegment* segment = reinterpret_cast<HeapSegment*>(address) - 1;
		segment->free = true;
		segment->combineForward();
		segment->combineBackward();
	}
}
export
{
	void* operator new(size_t size)
	{
		return memory::alloc(size);
	}
	void* operator new[](size_t size)
	{
		return memory::alloc(size);
	}
	void operator delete(void* p)
	{
		memory::free(p);
	}
	void operator delete(void* p, u64 size)
	{
		memory::free(p);
	}
	void operator delete[](void* p)
	{
		memory::free(p);
	}
	void operator delete[](void* p, u64 size)
	{
		memory::free(p);
	}
}