export module sl.allocator;
import sl.type;
import sl.typetraits;
export namespace std
{
	template <class T>
	struct allocator
	{
		using _From_primary = allocator;
		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using propagate_on_container_move_assignment = true_type;
		constexpr allocator() noexcept {}
		constexpr allocator(const allocator&) noexcept = default;
		template <class U>
		constexpr allocator(const allocator<U>& other) noexcept {}
		constexpr ~allocator() = default;
		constexpr allocator& operator=(const allocator&) = default;
		[[nodiscard]] constexpr __declspec(allocator) T* allocate(size_t n) const
		{
			static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
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