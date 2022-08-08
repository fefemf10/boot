export module memory:utils;
import types;
export namespace memory
{
	void set(void* p, u8 value, u64 size)
	{
		for (u64 i = 0; i < size; ++i)
		{
			*reinterpret_cast<u8*>(reinterpret_cast<u64>(p) + i) = value;
		}
	}
	void copy(void* source, void* destination, u64 size)
	{
		for (u64 i = 0; i < size; ++i)
		{
			reinterpret_cast<u8*>(destination)[i] = reinterpret_cast<u8*>(source)[i];
		}
	}
}