export module memory;
import memory.allocator;
import memory.descriptor;
import memory.PageTable;
import memory.PageTableManager;
import memory.utils;
import memory.Heap;
import types;
import cpuio;
import BootInfo;
import console;
import intrinsic;

export namespace memory
{
	const char* types[] {
		"EfiReservedMemoryType",
		"EfiLoaderCode",
		"EfiLoaderData",
		"EfiBootServicesCode",
		"EfiBootServicesData",
		"EfiRuntimeServicesCode",
		"EfiRuntimeServicesData",
		"EfiConventionalMemory",
		"EfiUnusableMemory",
		"EfiACPIReclaimMemory",
		"EfiACPIMemoryNVS",
		"EfiMemoryMappedIO",
		"EfiMemoryMappedIOPortSpace",
		"EfiPalCode",
		"EfiPersistentMemory",
		"EfiMaxMemoryType"
	};
	extern void loadPLM(PageTable* plm4);
	void initialize(const BootInfo& bootInfo)
	{
		allocator::initialize(bootInfo.map, bootInfo.mapEntries, bootInfo.descriptorSize);
		if (reinterpret_cast<u64>(bootInfo.fb.baseAddress) < sizeRAM)
			allocator::reserveBlocks(bootInfo.fb.baseAddress, allocator::countBlocks(bootInfo.fb.bufferSize));
		for (size_t i = 0; i < bootInfo.mapEntries; i++)
		{
			const Descriptor* descriptor = (Descriptor*)((u64)bootInfo.map + (i * bootInfo.descriptorSize));
			switch (descriptor->type)
			{
			case Descriptor::RESERVEDMEMORY_TYPE:
			case Descriptor::RUNTIMESERVICES_CODE:
			case Descriptor::RUNTIMESERVICES_DATA:
			case Descriptor::UNUSABLE_MEMORY:
			case Descriptor::ACPIRECLAIM_MEMORY:
			case Descriptor::ACPIMEMORYNVS:
			case Descriptor::MEMORYMAPPEDIO:
			case Descriptor::MEMORYMAPPEDIOPORTSPACE:
			case Descriptor::PALCODE:
			case Descriptor::PERSISTENT_MEMORY:
			case Descriptor::MAXMEMORY_TYPE:
				allocator::reserveBlocks(descriptor->physicalAddress, descriptor->numberOfPages);
				break;
			}
		}
		
		allocator::reserveBlocks(nullptr, 1);
		//countPagesForInitAP check efi_main.cpp in boot project
		allocator::setRegion((const void*)0x1000, 5);
		for (size_t i = 0; i < 4; i++)
		{
			allocator::setRegion(bootInfo.memoryMapEntries[i].address, bootInfo.memoryMapEntries[i].numberOfPages);
		}

		PLM4 = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
		set(PLM4, 0, sizeof(PageTable));
		pageTableManager = PageTableManager(PLM4);
		
		pageTableManager.mapMemory((void*)0x1000, (void*)0x1000, (allocator::maxBlocks) - 1);
		pageTableManager.mapMemory(bootInfo.fb.baseAddress, bootInfo.fb.baseAddress, allocator::countBlocks(bootInfo.fb.bufferSize));
		__writecr3(reinterpret_cast<u64>(PLM4));
		initializeHeap((void*)0x0000100000000000, 0x10);
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