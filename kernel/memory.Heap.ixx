export module memory.Heap;
import types;
import memory.allocator;
import memory.PageTableManager;
import memory.PageTable;
export namespace memory
{
	struct HeapSegment;
	HeapSegment* lastHS{};
	PageTable* PLM4;
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
			next = next->next;
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
			if (next)
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

	void initializeHeap(void* address, u64 countPages)
	{
		PageTableManager pageTableManager(PLM4);
		const u64 heapSize = countPages * memory::PAGESIZE;
		void* s = memory::allocator::allocBlocks(countPages);
		pageTableManager.mapMemory(s, address, countPages);
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
		if (size % memory::PAGESIZE)
		{
			size -= size % memory::PAGESIZE;
			size += memory::PAGESIZE;
		}
		u64 pages = size / memory::PAGESIZE;
		HeapSegment* newSegment = reinterpret_cast<HeapSegment*>(heapEnd);
		PageTableManager pageTableManager(PLM4);
		pageTableManager.mapMemory(allocator::allocBlocks(pages), heapEnd, pages);
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