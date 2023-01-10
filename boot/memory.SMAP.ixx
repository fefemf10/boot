export module memory.SMAP;
import types;
import console;
import memory.allocator;
export namespace memory
{
	struct SMAP
	{
		u64* base;
		u64 length;
		u32 type;
		u32 acpi;
		constexpr static u64 address = 0x27C00;
		static u32 size;
		static SMAP* smap;
		static void initialize()
		{
			size = *reinterpret_cast<u32*>(SMAP::address);
			smap = reinterpret_cast<SMAP*>(SMAP::address + 4);
		}
	};
	u32 SMAP::size;
	SMAP* SMAP::smap;
	void printSMAP()
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
	}
}