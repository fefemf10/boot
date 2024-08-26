export module sl.array;
import sl.iterator;
import sl.memory;
import sl.type;
export namespace std
{
	template <class T, size_t N>
	class array
	{
	public:
		static_assert(is_object_v<T>, "The C++ Standard forbids containers of non-object types "
			"because of [container.requirements].");
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        using iterator = linear_iterator<T>;
        using const_iterator = const_linear_iterator<T>;

        //using reverse_iterator = _STD reverse_iterator<iterator>;
        //using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

        constexpr void fill(const T& value)
        {
            std::fill_n(m_data, N, value);
        }
		constexpr void swap(array& other) noexcept(_Is_nothrow_swappable<T>::value) {
			std::_Swap_ranges_unchecked(m_data, m_data + N, other.m_data);
		}
		[[nodiscard]] constexpr T* data() noexcept { return m_data; }
		[[nodiscard]] constexpr const T* data() const noexcept { return m_data; }
		[[nodiscard]] constexpr size_type size() const noexcept { return N; }
		[[nodiscard]] constexpr bool empty() const noexcept { return false; }
		[[nodiscard]] constexpr size_type max_size() const noexcept { return N; }

		[[nodiscard]] constexpr reference operator[](const size_type pos) noexcept { return m_data[pos]; }
		[[nodiscard]] constexpr const_reference operator[](const size_type pos) const noexcept { return m_data[pos]; }
		[[nodiscard]] constexpr iterator begin() noexcept { return iterator(m_data); }
		[[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator(m_data); }
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return begin(); }
		[[nodiscard]] constexpr iterator end() noexcept { return iterator(m_data, N); }
		[[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator(m_data, N); }
		[[nodiscard]] constexpr const_iterator cend() const noexcept { return end(); }
		[[nodiscard]] constexpr reference front() noexcept { return m_data[0]; }
		[[nodiscard]] constexpr const_reference front() const noexcept { return m_data[0]; }
		[[nodiscard]] constexpr reference back() noexcept { return m_data[N - 1]; }
		[[nodiscard]] constexpr const_reference back() const noexcept { return m_data[N - 1]; }
	private:
        T m_data[N];
	};
}