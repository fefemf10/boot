export module sl.vector;
import sl.allocator;
import sl.typetraits;
import sl.numeric_limits;
import sl.memory;
import sl.iterator;
import sl.typetraits;
import sl.concepts;
import sl.type;

export namespace std
{
	template <class T, class Allocator = allocator<T>>
	class vector
	{
	public:
		using value_type = T;
		using allocator_type = Allocator;
		using pointer = typename allocator_traits<Allocator>::pointer;
		using const_pointer = typename allocator_traits<Allocator>::const_pointer;
		using reference = T&;
		using const_reference = const T&;
		using size_type = typename allocator_traits<Allocator>::size_type;
		using difference_type = typename allocator_traits<Allocator>::difference_type;
	
		using iterator = linear_iterator<vector<T>>;
		using const_iterator = const_linear_iterator<vector<T>>;
		//using reverse_iterator = reverse_iterator<iterator>;
		//using const_reverse_iterator = reverse_iterator<const_iterator>;
		constexpr vector() noexcept(noexcept(Allocator())) {}
		constexpr explicit vector(const Allocator& alloc) noexcept {}
		constexpr vector(const size_t count, const T& value, const Allocator& alloc = Allocator()) : m_first(alloc.allocate(count)), m_last(m_first + count), m_end(m_first + count)
		{
			for (size_t i = 0; i < count; ++i)
				m_first[i] = value;
		}
		constexpr explicit vector(const size_t count, const Allocator& alloc = Allocator()) : m_first(alloc.allocate(count)), m_last(m_first), m_end(m_first + count)
		{
			
		}
		constexpr vector(const vector& other) : vector(other, Allocator())
		{
		}
		constexpr vector(const vector& other, const Allocator& alloc)
		{
			m_first = alloc.allocate(other.capacity());
			for (size_t i = 0; i < other.size(); ++i)
				m_first[i] = other.m_first[i];
		}
		constexpr vector(vector&& other) noexcept;
		constexpr vector(vector&& other, const Allocator& alloc);
		/*template <class... U>
		constexpr T& emplace_back(U&&... value)
		{
			return emplace_one_at_back(std::forward(value)...);
		}
		constexpr void push_back(const T& value)
		{
			emplace_one_at_back(value);
		}*/
		[[nodiscard]] constexpr T* data() noexcept
		{
			return m_first;
		}
		[[nodiscard]] constexpr const T* data() const noexcept
		{
			return m_first;
		}
		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return static_cast<size_type>(m_last - m_first);
		}
		[[nodiscard]] constexpr size_type capacity() const noexcept
		{
			return static_cast<size_type>(m_end - m_first);
		}
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return m_first == m_last;
		}
		[[nodiscard]] constexpr size_type max_size() const noexcept
		{
			return numeric_limits<size_type>::max();
		}
		constexpr void reserve(const size_type capacity)
		{
			if (capacity > static_cast<size_type>(m_end - m_first))
			{ 
				reallocateExactly(capacity);
			}
		}
		[[nodiscard]] constexpr void clear() noexcept
		{

		}
		[[nodiscard]] constexpr T& operator[](const size_type pos) noexcept
		{
			return m_first[pos];
		}
		[[nodiscard]] constexpr const T& operator[](const size_type pos) const noexcept
		{
			return m_first[pos];
		}
		[[nodiscard]] constexpr iterator begin() noexcept
		{
			return iterator(m_first, std::addressof(*this));
		}
		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
			return const_iterator(m_first, std::addressof(*this));
		}
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}
		[[nodiscard]] constexpr iterator end() noexcept
		{
			return iterator(m_last, std::addressof(*this));
		}
		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
			return const_iterator(m_last, std::addressof(*this));
		}
		[[nodiscard]] constexpr const_iterator cend() const noexcept
		{
			return end();
		}
		[[nodiscard]] constexpr T& front(const size_t pos) noexcept
		{
			return *m_first;
		}
		[[nodiscard]] constexpr const T& front(const size_t pos) const noexcept
		{
			return *m_first;
		}
		[[nodiscard]] constexpr T& back(const size_t pos) noexcept
		{
			return *(m_last - 1);
		}
		[[nodiscard]] constexpr const T& back(const size_t pos) const noexcept
		{
			return *(m_last - 1);
		}
	private:
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
			alloc.deallocate(m_first, this->capacity());
			m_first = newVector;
			m_last = m_first + newSize;
			m_end = m_first + capacity;
		}
		//template <class... U>
		//constexpr T& emplace_one_at_back(U&&... value)
		//{
		//	if (m_last != m_end)
		//		return emplace_back_with_unused_capacity(std::forward<U>(value)...);
		//	return *emplace_reallocate(m_last, std::forward<U>(value)...);
		//}
		//template <class... U>
		//constexpr T& emplace_back_with_unused_capacity(U&&... value)
		//{
		//	if constexpr (conjunction_v<is_nothrow_constructible<T, U...>, _Uses_default_construct<Allocator, T*, U...>>)
		//		_Construct_in_place(*m_last, std::forward<U>(value)...);
		//	else
		//		allocator_traits<Allocator>::construct(alloc, _Unfancy(m_last), std::forward<U>(value)...);
		//	T& result = *m_last;
		//	++m_last;
		//	return result;
		//}
		//template <class... U>
		//constexpr pointer emplace_reallocate(const pointer _Whereptr, U&&... value)
		//{
		//	const auto _Whereoff = static_cast<size_type>(_Whereptr - m_first);

		//	const size_type newsize = size() + 1;
		//	const size_type newcapacity = calculateGrow(newsize);

		//	const pointer newvec = alloc.allocate(newcapacity);
		//	const pointer _Constructed_last = newvec + _Whereoff + 1;
		//	pointer _Constructed_first = _Constructed_last;

		//	allocator_traits<Allocator>::construct(alloc, _Unfancy(newvec + _Whereoff), std::forward<U>(value)...);
		//	_Constructed_first = newvec + _Whereoff;

		//	//if (_Whereptr == m_last)
		//	//{ // at back, provide strong guarantee
		//	//	if constexpr (is_nothrow_move_constructible_v<T> || !is_copy_constructible_v<T>) {
		//	//		_Uninitialized_move(_Myfirst, _Mylast, newvec, _Al);
		//	//	}
		//	//	else {
		//	//		_Uninitialized_copy(_Myfirst, _Mylast, newvec, _Al);
		//	//	}
		//	//}
		//	//else { // provide basic guarantee
		//	//	_Uninitialized_move(_Myfirst, _Whereptr, newvec, _Al);
		//	//	_Constructed_first = newvec;
		//	//	_Uninitialized_move(_Whereptr, _Mylast, newvec + _Whereoff + 1, _Al);
		//	//}
		//	//	_Destroy_range(_Constructed_first, _Constructed_last, _Al);
		//	//_Al.deallocate(newvec, newcapacity);
		//	//_RERAISE;
		//	//_CATCH_END

		//	//	_Change_array(newvec, newsize, newcapacity);
		//	return newvec + _Whereoff;
		//}
		T* m_first{};
		T* m_last{};
		T* m_end{};
		Allocator alloc;
	};
}