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
	/*PageTableManager pageTableManager;
	extern "C" void loadGDT(PageTable* plm4);*/
	void initializeHeap(void* address, u64 size);
	
	/*{
		
		set(PLM4, 0, sizeof(PageTable));
		pageTableManager = PageTableManager(PLM4);
		for (u64 i = 0; i < sizeRAM; i += 0x1000)
		{
			pageTableManager.mapMemory((void*)i, (void*)i);
		}
		pageTableManager.mapMemory((void*)fb, (void*)fb, fbSize + 0x1000);
		loadGDT(PLM4);
		initializeHeap(PLM4, (void*)0x0000100000000000, 0x10);
	}*/
}
//export
//{
//	void* operator new(size_t size)
//	{
//		return memory::alloc(size);
//	}
//	void* operator new[](size_t size)
//	{
//		return memory::alloc(size);
//	}
//	void operator delete(void* p)
//	{
//		memory::free(p);
//	}
//	void operator delete(void* p, u64 size)
//	{
//		memory::free(p);
//	}
//	void operator delete[](void* p)
//	{
//		memory::free(p);
//	}
//	void operator delete[](void* p, u64 size)
//	{
//		memory::free(p);
//	}
//}