export module vfs.Directory;
import types;

export namespace vfs
{
	struct Directory
	{
		u64 numberOfEntries;
		Directory* directories;
	};
}