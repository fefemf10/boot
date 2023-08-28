export module memory.PageTable;
import types;

export namespace memory
{
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
}