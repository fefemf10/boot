export module memory.descriptor;
import types;
export namespace memory
{
	struct Descriptor
	{
		enum Type : u32
		{
			RESERVEDMEMORY_TYPE,
			LOADER_CODE,
			LOADER_DATA,
			BOOTSERVICES_CODE,
			BOOTSERVICES_DATA,
			RUNTIMESERVICES_CODE,
			RUNTIMESERVICES_DATA,
			CONVENTIONAL_MEMORY,
			UNUSABLE_MEMORY,
			ACPIRECLAIM_MEMORY,
			ACPIMEMORYNVS,
			MEMORYMAPPEDIO,
			MEMORYMAPPEDIOPORTSPACE,
			PALCODE,
			PERSISTENT_MEMORY,
			MAXMEMORY_TYPE
		};
		Type type;
		u32 pad;
		void* physicalAddress;
		void* virtualAddress;
		u64 numberOfPages;
		u64 attribute;
	};

	/*void printSMAP()
	{
		for (u64 i = 0; i < SMAP::size; i++)
		{
			console::printf(u8"Region: %x ", i);
			console::printf(u8"base: %llx ", SMAP::smap[i].base);
			console::printf(u8"length: %llx ", SMAP::smap[i].length);
			console::printf(u8"type: %x ", SMAP::smap[i].type);
			console::printf(u8"acpi: %x ", SMAP::smap[i].acpi);
			switch (SMAP::smap[i].type)
			{
			case 1: console::puts(u8" (Available)\n"); break;
			case 2: console::puts(u8" (Reserved)\n"); break;
			case 3: console::puts(u8" (ACPI Reclaim)\n"); break;
			case 4: console::puts(u8" (ACPI NVS Memory)\n"); break;
			default: console::puts(u8" (Reserved)\n"); break;
			}
		}
		console::printf(u8"Total RAM: %llx\n", reinterpret_cast<u64>(SMAP::smap[SMAP::size - 1].base) + SMAP::smap[SMAP::size - 1].length - 1);
		console::printf(u8"Total 4KB blocks: %x\n", allocator::maxBlocks);
		console::printf(u8"Used blocks: %x\n", allocator::usedBlocks);
		console::printf(u8"Free blocks: %x\n", allocator::maxBlocks - allocator::usedBlocks);
	}*/
}