export module memory;
import memory.allocator;
import memory.descriptor;
import memory.PageTableManager;
import memory.utils;
import memory.Heap;
import types;
import cpuio;
import BootInfo;
import console;

export namespace memory
{
	const char8_t* types[] {
		u8"EfiReservedMemoryType",
		u8"EfiLoaderCode",
		u8"EfiLoaderData",
		u8"EfiBootServicesCode",
		u8"EfiBootServicesData",
		u8"EfiRuntimeServicesCode",
		u8"EfiRuntimeServicesData",
		u8"EfiConventionalMemory",
		u8"EfiUnusableMemory",
		u8"EfiACPIReclaimMemory",
		u8"EfiACPIMemoryNVS",
		u8"EfiMemoryMappedIO",
		u8"EfiMemoryMappedIOPortSpace",
		u8"EfiPalCode",
		u8"EfiPersistentMemory",
		u8"EfiMaxMemoryType"
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
			case 0:
			case 5:
			case 6:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				allocator::reserveBlocks(descriptor->physicalAddress, descriptor->numberOfPages);
				break;
			}
			
		}
		
		allocator::reserveBlocks(nullptr, 1);
		//allocator::setRegion((const void*)0xA0000, 0x10);
		for (size_t i = 0; i < 4; i++)
		{
			allocator::setRegion(bootInfo.memoryMapEntries[i].address, bootInfo.memoryMapEntries[i].numberOfPages);
		}

		PLM4 = reinterpret_cast<PageTable*>(allocator::allocBlocks(1));
		set(PLM4, 0, sizeof(PageTable));
		pageTableManager = PageTableManager(PLM4);
		
		//pageTableManager.mapMemory((void*)0x1000, (void*)0x1000, 0x10000-1);
		//pageTableManager.mapMemory((void*)0x1000, (void*)0x1000, 0x12BFF);
		//pageTableManager.mapMemory((void*)0x1000, (void*)0x1000, (allocator::maxBlocks) - 1);
		for (size_t i = 0; i < bootInfo.mapEntries; i++)
		{
			const Descriptor* descriptor = (Descriptor*)((u64)bootInfo.map + (i * bootInfo.descriptorSize));
			switch (descriptor->type)
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 7:
				pageTableManager.mapMemory(descriptor->physicalAddress, descriptor->physicalAddress, descriptor->numberOfPages);
				allocator::reserveBlocks(descriptor->physicalAddress, descriptor->numberOfPages);
				break;
			}

		}
		pageTableManager.mapMemory(bootInfo.fb.baseAddress, bootInfo.fb.baseAddress, allocator::countBlocks(bootInfo.fb.bufferSize));
		//cpuio::loadPLM(PLM4);
		//initializeHeap((void*)0x0000100000000000, 0x10);
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