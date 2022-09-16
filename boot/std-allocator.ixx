export module std:allocator;

export namespace std
{
	template <class T>
	struct allocator
	{
		constexpr allocator() noexcept = default;
		/*template <class U>
		constexpr allocator(const allocator<U>& other) noexcept;*/
		constexpr ~allocator() = default;
		[[nodiscard]] constexpr T* allocate(size_t n) const
		{
			return new T[n];
		}
		constexpr void deallocate(T* p, size_t n) const
		{
			delete[] p;
		}
	};
	template <class T1, class T2>
	constexpr bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs) noexcept
	{
		return lhs == rhs;
	}
}