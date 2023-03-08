export module memory.utils;
import types;
export namespace memory
{
	void set(void* p, u8 value, u64 size);
	void copy(void* destination, const void* source, u64 size);
	void move(void* destination, const void* source, u64 size);
}