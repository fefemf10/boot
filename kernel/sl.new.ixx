export module sl.neww;
import types;
import memory.Heap;
export namespace std
{
	enum class align_val_t : size_t {};
	struct nothrow_t
	{
		explicit nothrow_t() = default;
	};
	extern nothrow_t const nothrow;
}
export
{
	__declspec(allocator) void* operator new(size_t size)
	{
		return memory::alloc(size);
	}
	__declspec(allocator) void* operator new(size_t size, const std::nothrow_t&) noexcept
	{
		return memory::alloc(size);
	}
	inline void* operator new(size_t size, void* ptr) noexcept
	{
		return ptr;
	}
	__declspec(allocator) void* operator new[](size_t size)
	{
		return memory::alloc(size);
	}
	__declspec(allocator) void* operator new[](size_t size, const std::nothrow_t&) noexcept
	{
		return memory::alloc(size);
	}
	inline void* operator new[](size_t size, void* ptr) noexcept
	{
		return ptr;
	}
	void operator delete(void* p) noexcept
	{
		memory::free(p);
	}
	void operator delete(void* p, const std::nothrow_t&) noexcept
	{
		memory::free(p);
	}
	void operator delete(void* p, u64 size) noexcept
	{
		memory::free(p);
	}
	inline void operator delete(void*, void*) noexcept
	{
		return;
	}
	void operator delete[](void* p) noexcept
	{
		memory::free(p);
	}
	void operator delete[](void* p, const std::nothrow_t&) noexcept
	{
		memory::free(p);
	}
	void operator delete[](void* p, u64 size) noexcept
	{
		memory::free(p);
	}
	inline void operator delete[](void*, void*) noexcept
	{
		return;
	}



	void* operator new(size_t size, std::align_val_t al)
	{
		return memory::alloc(size);
	}
	__declspec(allocator) void* operator new(size_t size, std::align_val_t al, std::nothrow_t const&) noexcept
	{
		return memory::alloc(size);
	}
	void* operator new[](size_t size, std::align_val_t al)
	{
		return memory::alloc(size);
	}
	__declspec(allocator) void* operator new[](size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
	{
		return memory::alloc(size);
	}
	void operator delete(void* p, std::align_val_t al) noexcept
	{
		memory::free(p);
	}
	void operator delete(void* p, std::align_val_t al, const std::nothrow_t&) noexcept
	{
		memory::free(p);
	}
	void operator delete(void* p, u64 size, std::align_val_t al) noexcept
	{
		memory::free(p);
	}
	void operator delete[](void* p, std::align_val_t al) noexcept
	{
		memory::free(p);
	}
	void operator delete[](void* p, std::align_val_t al, const std::nothrow_t&) noexcept
	{
		memory::free(p);
	}
	void operator delete[](void* p, u64 size, std::align_val_t al) noexcept
	{
		memory::free(p);
	}
}