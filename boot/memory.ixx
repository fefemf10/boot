export module memory;
import memory.utils;
import memory.allocator;
import memory.PageIndex;
import memory.PageTableManager;
import memory.Heap;
import memory.SMAP;
import VESA;
import types;
import cpuio;

export namespace memory
{
	constexpr u64 KERNELSIZE = 0x1000 * 64;//64 pages for kernel
	PageTableManager pageTableManager;
	extern "C" void loadGDT(PageTable * plm4);
	void initializeHeap(void* address, u64 size);
	
	void initialize()
	{
		SMAP::initialize();
		u64 totalRAM = reinterpret_cast<u64>(SMAP::smap[SMAP::size - 1].base) + SMAP::smap[SMAP::size - 1].length - 1ull;
		allocator::initialize(reinterpret_cast<u64*>(0x40000), totalRAM);
		for (u32 i = 0; i < SMAP::size; ++i)
		{
			if (SMAP::smap[i].type == 1)
			{
				allocator::setRegion(SMAP::smap[i].base, SMAP::smap[i].length);
			}
		}
		//Reverse memory for this allocator map
		allocator::unsetRegion(reinterpret_cast<u64*>(0x40000), allocator::maxBlocks >> allocator::BLOCKSPERBYTE);
		allocator::unsetRegion(reinterpret_cast<u64*>(0), 0x7C00);
		allocator::unsetRegion(reinterpret_cast<u64*>(SMAP::address), sizeof(SMAP::size) + SMAP::size * sizeof(SMAP));
		allocator::unsetRegion(reinterpret_cast<u64*>(VESA::VESAInfo::address), 0x10000);
		allocator::unsetRegion(reinterpret_cast<u64*>(0x100000), KERNELSIZE);
		PLM4 = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
		set(PLM4, 0, sizeof(PageTable));
		pageTableManager = PageTableManager(PLM4);
		for (u64 i = 0; i < 0x8000000/*128mb*/; i += 0x1000)
		{
			pageTableManager.mapMemory((void*)i, (void*)i);
		}
		loadGDT(PLM4);
		initializeHeap(PLM4, (void*)0x0000100000000000, 0x10);
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