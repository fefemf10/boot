export module memory;
import memory.allocator;
import memory.descriptor;
import memory.PageTableManager;
import memory.utils;
import memory.Heap;
import console;
import types;


export namespace memory
{
	PageTableManager pageTableManager;
	extern "C" void loadGDT(PageTable* plm4);
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