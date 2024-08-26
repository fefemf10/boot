export module sl.compat;

export
{
	extern "C"
	{
		[[nodiscard]] constexpr int memcmp(const void* dst, const void* src, size_t size);
		[[nodiscard]] constexpr void* memset(void* p, unsigned char value, size_t size);
		[[nodiscard]] constexpr void* memcpy(void* dst, const void* src, size_t size);
		[[nodiscard]] constexpr void* memmove(void* dst, const void* src, size_t size);
	}
}