export module memory.PageIndex;
import types;
export namespace memory
{
	struct PageIndex
	{
		PageIndex(u64 virtualAddress)
		{
			virtualAddress >>= 12;
			p = virtualAddress & 0x1FF;
			virtualAddress >>= 9;
			pt = virtualAddress & 0x1FF;
			virtualAddress >>= 9;
			pd = virtualAddress & 0x1FF;
			virtualAddress >>= 9;
			pdp = virtualAddress & 0x1FF;
		}
		u64 pdp;
		u64 pd;
		u64 pt;
		u64 p;
	};
}