export module sl.vector;
import sl.typetraits;
import sl.numeric_limits;
import sl.iterator;
import sl.memory;
import sl.utility;
import sl.type;
import sl.initializer_list;

export namespace std
{
	template <class T, class Allocator = allocator<T>>
	class vector
	{
	private:
		using Alty = Allocator;
		using Alty_traits = allocator_traits<Allocator>;
	public:
		static_assert(is_object_v<T>, "The C++ Standard forbids containers of non-object types "
			"because of [container.requirements].");
		using value_type = T;
		using allocator_type = Allocator;
		using pointer = typename Alty_traits::pointer;
		using const_pointer = typename Alty_traits::const_pointer;
		using reference = T&;
		using const_reference = const T&;
		using size_type = typename Alty_traits::size_type;
		using difference_type = typename Alty_traits::difference_type;

		using iterator = linear_iterator<vector<T>>;
		using const_iterator = const_linear_iterator<vector<T>>;
		//using reverse_iterator = reverse_iterator<iterator>;
		//using const_reverse_iterator = reverse_iterator<const_iterator>;
		constexpr vector() noexcept(is_nothrow_default_constructible_v<Alty>) {}
		constexpr explicit vector(const Alty& alloc) noexcept : alloc(alloc) {}
		constexpr vector(const size_t count, const T& value, const Alty& alloc = Alty()) : alloc(alloc)
		{
			construct_n(count, value);
		}
		constexpr explicit vector(const size_t count, const Alty& alloc = Alty()) : alloc(alloc)
		{
			construct_n(count);
		}
		constexpr vector(const vector& other) : vector(other, Alty())
		{
		}
		constexpr vector(const vector& other, const Alty& alloc)
		{
			m_first = alloc.allocate(other.capacity());
			for (size_t i = 0; i < other.size(); ++i)
				m_first[i] = other.m_first[i];
		}
		constexpr vector(std::initializer_list<T> list, const Alty& alloc = Alty()) : alloc(alloc)
		{
			construct_n(list.size(), list.begin(), list.end());
		}
		constexpr vector(vector&& other) noexcept;
		constexpr vector(vector&& other, const Alty& alloc);
		template <class... U>
		constexpr decltype(auto) emplace_back(U&&... value)
		{
			T& result = emplace_one_at_back(std::forward<U>(value)...);
			return result;
		}
		constexpr void push_back(const T& value) { emplace_one_at_back(value); }
		constexpr void push_back(T&& value) { emplace_one_at_back(std::move(value)); }
		[[nodiscard]] constexpr T* data() noexcept { return m_first; }
		[[nodiscard]] constexpr const T* data() const noexcept { return m_first; }
		[[nodiscard]] constexpr size_type size() const noexcept { return static_cast<size_type>(m_last - m_first); }
		[[nodiscard]] constexpr size_type capacity() const noexcept { return static_cast<size_type>(m_end - m_first); }
		[[nodiscard]] constexpr bool empty() const noexcept { return m_first == m_last; }
		[[nodiscard]] constexpr size_type max_size() const noexcept { return numeric_limits<size_type>::max(); }
		constexpr void reserve(const size_type capacity)
		{
			if (capacity > static_cast<size_type>(m_end - m_first))
			{
				reallocateExactly(capacity);
			}
		}
		[[nodiscard]] constexpr void clear() noexcept
		{
			_Destroy_range(m_first, m_last, alloc);
			m_last = m_first;
		}
		[[nodiscard]] constexpr reference operator[](const size_type pos) noexcept { return m_first[pos]; }
		[[nodiscard]] constexpr const_reference operator[](const size_type pos) const noexcept { return m_first[pos]; }
		[[nodiscard]] constexpr iterator begin() noexcept { return iterator(m_first); }
		[[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator(m_first); }
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return begin(); }
		[[nodiscard]] constexpr iterator end() noexcept { return iterator(m_last); }
		[[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator(m_last); }
		[[nodiscard]] constexpr const_iterator cend() const noexcept { return end(); }
		[[nodiscard]] constexpr reference front() noexcept { return *m_first; }
		[[nodiscard]] constexpr const_reference front() const noexcept { return *m_first; }
		[[nodiscard]] constexpr reference back() noexcept { return *(m_last - 1); }
		[[nodiscard]] constexpr const_reference back() const noexcept { return *(m_last - 1); }
	public:
		constexpr size_t calculateGrow(const size_type newSize) const noexcept
		{
			if (capacity() > max_size() >> 1)
				return max_size();
			const size_t geometric = capacity() + capacity() / 2;
			return (geometric < newSize) ? newSize : geometric;
		}
		constexpr void reallocateExactly(const size_type capacity)
		{
			T* newVector = alloc.allocate(capacity);
			size_type newSize = size();
			for (size_t i = 0; i < newSize; i++)
				newVector[i] = m_first[i];
			if (m_first)
				alloc.deallocate(m_first, this->capacity());
			m_first = newVector;
			m_last = m_first + newSize;
			m_end = m_first + capacity;
		}
		template <class... U>
		constexpr void construct_n(const size_type count, U&&... values) {
			// Dispatches between the three sized constructions.
			// 1-arg -> value-construction, e.g. vector(5)
			// 2-arg -> fill, e.g. vector(5, "meow")
			// 3-arg -> sized range construction, e.g. vector{"Hello", "Fluffy", "World"}
			if (count != 0) {
				m_first = alloc.allocate(count);
				m_last = m_first + count;
				m_end = m_first + count;
				if constexpr (sizeof...(values) == 0)
					m_last = std::_Uninitialized_value_construct_n(m_first, count, alloc);
				else if constexpr (sizeof...(values) == 1)
					m_last = std::_Uninitialized_fill_n(m_first, count, values..., alloc);
				else if constexpr (sizeof...(values) == 2)
					m_last = std::_Uninitialized_copy(std::forward<U>(values)..., m_first, alloc);
			}
		}
		template <class... U>
		constexpr T& emplace_one_at_back(U&&... value)
		{
			if (m_last != m_end)
				return emplace_back_with_unused_capacity(std::forward<U>(value)...);
			return *emplace_reallocate(m_last, std::forward<U>(value)...);
		}
		template <class... U>
		constexpr T& emplace_back_with_unused_capacity(U&&... value)
		{
			if constexpr (conjunction_v<is_nothrow_constructible<T, U...>, _Uses_default_construct<Alty, T*, U...>>)
				_Construct_in_place(*m_last, std::forward<U>(value)...);
			else
				Alty_traits::construct(alloc, m_last, std::forward<U>(value)...);
			T& result = *m_last;
			++m_last;
			return result;
		}
		constexpr void _Change_array(const pointer _Newvec, const size_type _Newsize, const size_type _Newcapacity)
		{
			if (m_first)
			{
				_Destroy_range(m_first, m_last, alloc);
				alloc.deallocate(m_first, static_cast<size_type>(m_end - m_first));
			}

			m_first = _Newvec;
			m_last = _Newvec + _Newsize;
			m_end = _Newvec + _Newcapacity;
		}
		template <class... U>
		constexpr pointer emplace_reallocate(const pointer _Whereptr, U&&... value)
		{
			const auto _Whereoff = static_cast<size_type>(_Whereptr - m_first);

			const size_type newsize = size() + 1;
			const size_type newcapacity = calculateGrow(newsize);

			const pointer newvec = alloc.allocate_at_least(newcapacity).ptr;
			const pointer _Constructed_last = newvec + _Whereoff + 1;
			pointer _Constructed_first = _Constructed_last;

			Alty_traits::construct(alloc, newvec + _Whereoff, std::forward<U>(value)...);
			_Constructed_first = newvec + _Whereoff;
			if (_Whereptr == m_last)
			{
				if constexpr (is_nothrow_move_constructible_v<T> || !is_copy_constructible_v<T>) {
					_Uninitialized_move(m_first, m_last, newvec, alloc);
				}
				else {
					_Uninitialized_copy(m_first, m_last, newvec, alloc);
				}
			}
			else { // provide basic guarantee
				_Uninitialized_move(m_first, _Whereptr, newvec, alloc);
				_Constructed_first = newvec;
				_Uninitialized_move(_Whereptr, m_last, newvec + _Whereoff + 1, alloc);
			}
			_Change_array(newvec, newsize, newcapacity);
			return newvec + _Whereoff;
		}
		T* m_first{};
		T* m_last{};
		T* m_end{};
		Alty alloc;
	};
}