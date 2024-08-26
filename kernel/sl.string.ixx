export module sl.string;
import sl.bit;
import sl.algorithm;
import sl.numeric_limits;
import sl.initializer_list;
import sl.utility;
import sl.memory;
import sl.iterator;
import sl.chartraits;
import sl.concepts;
import sl.type;
import sl.typetraits;
export namespace std
{
	enum class ConstructStrategy : uint8_t { FromChar, FromPtr, FromString };
	template<class T, class Traits = char_traits<T>, class Allocator = allocator<T>>
	class basic_string
	{
		using Alty = Allocator;
		using Alty_traits = allocator_traits<Allocator>;
	public:
		using value_type = T;
		using allocator_type = Allocator;
		using pointer = typename Alty_traits::pointer;
		using const_pointer = typename Alty_traits::const_pointer;
		using reference = T&;
		using const_reference = const T&;
		using size_type = typename Alty_traits::size_type;
		using difference_type = typename Alty_traits::difference_type;

		using iterator = linear_iterator<basic_string<T>>;
		using const_iterator = const_linear_iterator<basic_string<T>>;
		//using reverse_iterator = reverse_iterator<iterator>;
		//using const_reverse_iterator = reverse_iterator<const_iterator>;

		static_assert(is_same_v<T, typename Traits::char_type>,
			"N4950 [string.require]/3 requires that the supplied "
			"char_traits character type match the string's character type.");

		static_assert(!is_array_v<T>&& is_trivial_v<T>&& is_standard_layout_v<T>,
			"The character type of basic_string must be a non-array trivial standard-layout type. See N4950 "
			"[strings.general]/1.");

		static constexpr size_type SSOBufferSize = 16 / sizeof(char) < 1 ? 1 : 16 / sizeof(char);
		static constexpr size_type SSOCapacity = SSOBufferSize - 1;
		static constexpr auto npos{ static_cast<size_type>(-1) };
		static constexpr size_type _Alloc_mask = sizeof(value_type) <= 1 ? 15
			: sizeof(value_type) <= 2 ? 7
			: sizeof(value_type) <= 4 ? 3
			: sizeof(value_type) <= 8 ? 1
			: 0;
		static constexpr bool _Can_memcpy_val = _Is_specialization_v<Traits, char_traits>&& is_trivial_v<pointer>;
		template <class _Iter>
		using _Is_elem_cptr = bool_constant<Is_any_of_v<_Iter, const T* const, T* const, const T*, T*>>;

		constexpr basic_string() noexcept(is_nothrow_default_constructible_v<Alty>) { construct_empty(); }
		constexpr explicit basic_string(const Allocator& alloc) noexcept : alloc(alloc) { construct_empty(); }
		constexpr basic_string(const basic_string& other, const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			construct<ConstructStrategy::FromString>(other.data(), other.m_size);
		}
		constexpr basic_string(const basic_string& other, const size_type offset, const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			construct<ConstructStrategy::FromPtr>(other.data() + offset, clampSuffixSize(offset, npos));
		}
		constexpr basic_string(const basic_string& other, const size_type offset, const size_type count, const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			construct<ConstructStrategy::FromPtr>(other.data() + offset, clampSuffixSize(offset, count));
		}
		constexpr basic_string(basic_string&& other, const size_type offset, const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			moveConstructFromSubstr(other, offset, npos);
		}
		constexpr basic_string(basic_string&& other, const size_type offset, const size_type count, const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			moveConstructFromSubstr(other, offset, count);
		}
		constexpr basic_string(const T* ptr, const size_type count)
		{
			construct<ConstructStrategy::FromPtr>(ptr, count);
		}
		constexpr basic_string(const T* ptr)
		{
			construct<ConstructStrategy::FromPtr>(ptr, Traits::length(ptr));
		}
		basic_string(nullptr_t) = delete;
		constexpr basic_string(const size_type count, const T c)
		{
			construct<ConstructStrategy::FromChar>(c, count);
		}
		template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
		constexpr basic_string(_Iter _First, _Iter _Last, const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			auto _UFirst = _First;
			auto _ULast = _Last;
			if (_UFirst == _ULast) {
				construct_empty();
			}
			else {
				if constexpr (_Is_elem_cptr<decltype(_UFirst)>::value) {
					construct<ConstructStrategy::FromPtr>(_UFirst, static_cast<size_t>(_ULast - _UFirst));
				}
				else if constexpr (_Is_cpp17_fwd_iter_v<decltype(_UFirst)>) {
					const auto _Length = static_cast<size_t>(std::distance(_UFirst, _ULast));
					const auto count = _Length;
					constructFromIter(std::move(_UFirst), std::move(_ULast), count);
				}
				else
					constructFromIter(std::move(_UFirst), std::move(_ULast));
			}
		}
		constexpr basic_string(basic_string&& right) noexcept : alloc(right.alloc)
		{
			takeContents(right);
		}
		[[nodiscard]] constexpr T* data() noexcept
		{
			T* result = m_data.buffer;
			if (isLargeString())
				result = m_data.ptr;
			return result;
		}
		[[nodiscard]] constexpr const T* data() const noexcept
		{
			const T* result = m_data.buffer;
			if (isLargeString())
				result = m_data.ptr;
			return result;
		}
		[[nodiscard]] constexpr size_t size() const noexcept
		{
			return m_size;
		}
		[[nodiscard]] constexpr size_t capacity() const noexcept
		{
			return m_capacity;
		}
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return m_size == 0;
		}
		[[nodiscard]] constexpr size_t max_size() const noexcept
		{
			return numeric_limits<size_t>::max();
		}
		[[nodiscard]] constexpr void clear() noexcept
		{

		}
		[[nodiscard]] constexpr T& operator[](const size_t pos) noexcept
		{
			return data()[pos];
		}
		[[nodiscard]] constexpr const T& operator[](const size_t pos) const noexcept
		{
			return data()[pos];
		}
		[[nodiscard]] constexpr iterator begin() noexcept
		{
			return iterator(data());
		}
		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
			return const_iterator(data());
		}
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}
		[[nodiscard]] constexpr iterator end() noexcept
		{
			return iterator(data(), m_size);
		}
		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
			return const_iterator(data(), m_size);
		}
		[[nodiscard]] constexpr const_iterator cend() const noexcept
		{
			return end();
		}
		[[nodiscard]] constexpr T& front(const size_t pos) noexcept
		{
			return *data();
		}
		[[nodiscard]] constexpr const T& front(const size_t pos) const noexcept
		{
			return *data();
		}
		[[nodiscard]] constexpr T& back(const size_t pos) noexcept
		{
			return *(data() + m_size - 1);
		}
		[[nodiscard]] constexpr const T& back(const size_t pos) const noexcept
		{
			return *(data() + m_size - 1);
		}
		constexpr basic_string& operator=(basic_string&& right) noexcept
		{
			if (this == std::addressof(right))
				return *this;

			_Tidy_deallocate();
			takeContents(right);
			return *this;
		}
		constexpr basic_string& assign(basic_string&& right) noexcept(noexcept(*this = std::move(right)))
		{
			*this = std::move(right);
			return *this;
		}
		constexpr basic_string(initializer_list<T> _Ilist, const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			construct<ConstructStrategy::FromPtr>(_Ilist.begin(), _Ilist.size());
		}

		constexpr basic_string& operator=(initializer_list<T> _Ilist)
		{
			return assign(_Ilist.begin(), _Ilist.size());
		}

		constexpr basic_string& operator+=(initializer_list<T> _Ilist)
		{
			return append(_Ilist.begin(), _Ilist.size());
		}

		constexpr basic_string& assign(initializer_list<T> _Ilist)
		{
			return assign(_Ilist.begin(), _Ilist.size());
		}

		constexpr basic_string& append(initializer_list<T> _Ilist)
		{
			return append(_Ilist.begin(), _Ilist.size());
		}

		constexpr iterator insert(const const_iterator _Where, const initializer_list<T> _Ilist) {
			const auto _Off = static_cast<size_type>(_Where.ptr - data());
			insert(_Off, _Ilist.begin(), _Ilist.size());
			return begin() + static_cast<difference_type>(_Off);
		}

		constexpr basic_string& replace(const const_iterator _First, const const_iterator _Last, const initializer_list<T> _Ilist) {
			const auto _Offset = static_cast<size_type>(_First.ptr - data());
			const auto _Length = static_cast<size_type>(_Last.ptr - _First.ptr);
			return replace(_Offset, _Length, _Ilist.begin(), _Ilist.size());
		}
		constexpr ~basic_string() noexcept
		{
			_Tidy_deallocate();
		}
		constexpr basic_string& operator=(const basic_string& right) {
			if (this == std::addressof(right))
				return *this;

			assign(right.data(), right.m_size);
			return *this;
		}
		constexpr basic_string& operator=(const T* const ptr) {
			return assign(ptr);
		}

		basic_string& operator=(nullptr_t) = delete;

		constexpr basic_string& operator=(const T _Ch)
		{
			m_size = 1;
			T* const ptr = data();
			Traits::assign(ptr[0], _Ch);
			Traits::assign(ptr[1], T());
			return *this;
		}

		constexpr basic_string& operator+=(const basic_string& right) {
			return append(right);
		}

		constexpr basic_string& operator+=(const T* const ptr)
		{
			return append(ptr);
		}

		constexpr basic_string& operator+=(T c)
		{
			push_back(c);
			return *this;
		}

		constexpr basic_string& append(const basic_string& right)
		{
			return append(right.data(), right.m_size);
		}

		constexpr basic_string& append(const basic_string& right, const size_type _Roff, size_type _Count = npos) {
			_Count = right.clampSuffixSize(_Roff, _Count);
			return append(right.data() + _Roff, _Count);
		}

		constexpr basic_string& append(const T* const ptr, const size_type _Count) {
			const size_type _Old_size = m_size;
			if (_Count <= m_capacity - _Old_size)
			{
				m_size = _Old_size + _Count;
				T* const _Old_ptr = data();
				Traits::move(_Old_ptr + _Old_size, ptr, _Count);
				Traits::assign(_Old_ptr[_Old_size + _Count], T());
				return *this;
			}

			return _Reallocate_grow_by(
				_Count,
				[](T* const _New_ptr, const T* const _Old_ptr, const size_type _Old_size, const T* const ptr,
					const size_type _Count)
				{
					Traits::copy(_New_ptr, _Old_ptr, _Old_size);
					Traits::copy(_New_ptr + _Old_size, ptr, _Count);
					Traits::assign(_New_ptr[_Old_size + _Count], T());
				},
				ptr, _Count);
		}

		constexpr basic_string& append(const T* const ptr)
		{
			return append(ptr, Traits::length(ptr));
		}

		constexpr basic_string& append(const size_type _Count, const T _Ch)
		{
			const size_type _Old_size = m_size;
			if (_Count <= m_capacity - _Old_size) {
				m_size = _Old_size + _Count;
				T* const _Old_ptr = data();
				Traits::assign(_Old_ptr + _Old_size, _Count, _Ch);
				Traits::assign(_Old_ptr[_Old_size + _Count], T());
				return *this;
			}

			return _Reallocate_grow_by(
				_Count,
				[](T* const _New_ptr, const T* const _Old_ptr, const size_type _Old_size, const size_type _Count,
					const T _Ch)
				{
					Traits::copy(_New_ptr, _Old_ptr, _Old_size);
					Traits::assign(_New_ptr + _Old_size, _Count, _Ch);
					Traits::assign(_New_ptr[_Old_size + _Count], T());
				},
				_Count, _Ch);
		}

		template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
		constexpr basic_string& append(const _Iter _First, const _Iter _Last)
		{
			const auto _UFirst = _First;
			const auto _ULast = _Last;
			if constexpr (_Is_elem_cptr<decltype(_UFirst)>::value) {
				return append(_UFirst, static_cast<size_t>(_ULast - _UFirst));
			}
			else {
				const basic_string right(_UFirst, _ULast, Allocator());
				return append(right.data(), right.m_size);
			}
		}
		constexpr basic_string& assign(const basic_string& right)
		{
			*this = right;
			return *this;
		}

		constexpr basic_string& assign(const basic_string& right, const size_type _Roff, size_type _Count = npos)
		{
			_Count = right.clampSuffixSize(_Roff, _Count);
			return assign(right.data() + _Roff, _Count);
		}

		constexpr basic_string& assign(const T* const _Ptr, const size_type _Count) {
			if (_Count <= m_capacity)
			{
				T* const _Old_ptr = data();
				m_size = _Count;
				Traits::move(_Old_ptr, _Ptr, _Count);
				Traits::assign(_Old_ptr[_Count], T());
				return *this;
			}

			return _Reallocate_for(
				_Count,
				[](T* const _New_ptr, const size_type _Count, const T* const _Ptr)
				{
					Traits::copy(_New_ptr, _Ptr, _Count);
					Traits::assign(_New_ptr[_Count], T());
				},
				_Ptr);
		}

		constexpr basic_string& assign(const T* const _Ptr)
		{
			return assign(_Ptr, Traits::length(_Ptr));
		}

		constexpr basic_string& assign(const size_type _Count, const T _Ch) {
			if (_Count <= m_capacity)
			{
				T* const _Old_ptr = data();
				m_size = _Count;
				Traits::assign(_Old_ptr, _Count, _Ch);
				Traits::assign(_Old_ptr[_Count], T());
				return *this;
			}

			return _Reallocate_for(
				_Count,
				[](T* const _New_ptr, const size_type _Count, const T _Ch)
				{
					Traits::assign(_New_ptr, _Count, _Ch);
					Traits::assign(_New_ptr[_Count], T());
				},
				_Ch);
		}

		template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
		constexpr basic_string& assign(const _Iter _First, const _Iter _Last) {
			const auto _UFirst = _First;
			const auto _ULast = _Last;
			if constexpr (_Is_elem_cptr<decltype(_UFirst)>::value) {
				return assign(_UFirst, static_cast<size_t>(_ULast - _UFirst));
			}
			else {
				basic_string _Right(_UFirst, _ULast, Allocator());
				if (m_capacity < _Right.m_capacity) {
					_Mypair._Myval2._Orphan_all();
					_Swap_data(_Right);
					return *this;
				}
				else {
					return assign(_Right.data(), _Right.m_size);
				}
			}
		}

	private:
		enum class AllocationPolicy { AtLeast, Exactly };

		template <AllocationPolicy _Policy = AllocationPolicy::AtLeast>
		[[nodiscard]] static constexpr pointer AllocateForCapacity(Allocator& alloc, size_type& capacity)
		{
			++capacity; // Take null terminator into consideration
			pointer ptr = nullptr;
			if constexpr (_Policy == AllocationPolicy::AtLeast)
				ptr = alloc.allocate_at_least(capacity);
			else
				ptr = alloc.allocate(capacity);
			// Start element lifetimes to avoid UB. This is a more general mechanism than _String_val::_Activate_SSO_buffer,
			// but likely more impactful to throughput.
			if (std::is_constant_evaluated())
			{
				for (size_type i = 0; i < capacity; ++i)
					std::construct_at(ptr + i);
			}
			--capacity;
			return ptr;
		}

		static constexpr void DeallocateForCapacity(Allocator& alloc, const pointer old_ptr, const size_type capacity) noexcept
		{
			alloc.deallocate(old_ptr, capacity + 1); // +1 for null terminator
		}
		constexpr void construct_empty()
		{
			m_size = 0;
			m_capacity = SSOCapacity;
			activateSSO();
			Traits::assign(m_data.buffer[0], value_type());
		}
		template <ConstructStrategy strategy, class _Char_or_ptr>
		constexpr void construct(const _Char_or_ptr _Arg, const size_type count)
		{
			if (count <= SSOCapacity)
			{
				m_size = count;
				m_capacity = SSOCapacity;

				if constexpr (strategy == ConstructStrategy::FromChar) {
					Traits::assign(m_data.buffer, count, _Arg);
					Traits::assign(m_data.buffer[count], value_type());
				}
				else if constexpr (strategy == ConstructStrategy::FromPtr) {
					Traits::copy(m_data.buffer, _Arg, count);
					Traits::assign(m_data.buffer[count], value_type());
				}
				else { // strategy == ConstructStrategy::FromString
					Traits::copy(m_data.buffer, _Arg, SSOBufferSize);
				}
				return;
			}
			size_type _New_capacity = calculateGrow(count, SSOCapacity, max_size());
			const pointer _New_ptr = AllocateForCapacity(alloc, _New_capacity);
			_Construct_in_place(m_data.ptr, _New_ptr);

			m_size = count;
			m_capacity = _New_capacity;
			if constexpr (strategy == ConstructStrategy::FromChar) {
				Traits::assign(_New_ptr, count, _Arg);
				Traits::assign(_New_ptr[count], value_type());
			}
			else if constexpr (strategy == ConstructStrategy::FromPtr) {
				Traits::copy(_New_ptr, _Arg, count);
				Traits::assign(_New_ptr[count], value_type());
			}
			else { // strategy == ConstructStrategy::FromString
				Traits::copy(_New_ptr, _Arg, count + 1);
			}
		}

		template <class _Iter, class _Sent, class _Size = nullptr_t>
		constexpr void constructFromIter(_Iter _First, const _Sent _Last, _Size count = {})
		{
			m_size = 0;
			m_capacity = SSOCapacity;
			if constexpr (is_same_v<_Size, size_type>)
			{
				if (count > SSOCapacity) {
					size_type _New_capacity = calculateGrow(count);
					const pointer _New_ptr = AllocateForCapacity(alloc, _New_capacity);
					_Construct_in_place(m_data.ptr, _New_ptr);
					m_capacity = _New_capacity;
				}
			}

			constexpr bool _Can_construct_by_copy = _Is_specialization_v<Traits, char_traits>&& is_character_v<value_type>&& is_same_v<_Size, size_type>;

			if constexpr (_Can_construct_by_copy) {
				std::_Copy_n_unchecked4(std::move(_First), count, data());
				m_size = count;
				data()[count] = value_type();
			}
			else
			{
				for (; _First != _Last; ++_First)
				{
					if constexpr (!is_same_v<_Size, size_type>)
					{
						if (m_size == m_capacity)
						{
							value_type* const _Old_ptr = data();
							size_type _New_capacity = calculateGrow(m_size + 1);
							const pointer _New_ptr = AllocateForCapacity(alloc, _New_capacity);

							Traits::copy(_New_ptr, _Old_ptr, m_size);
							if (isLargeString())
							{
								DeallocateForCapacity(alloc, m_data.ptr, m_capacity);
								m_data.ptr = _New_ptr;
							}
							else {
								_Construct_in_place(m_data.ptr, _New_ptr);
							}
							m_capacity = _New_capacity;
						}
					}
					Traits::assign(data()[m_size], *_First);
					++m_size;
				}
				Traits::assign(data()[m_size], value_type());
			}
		}
		[[nodiscard]] constexpr bool isLargeString() const noexcept
		{
			return m_capacity > SSOCapacity;
		}
		constexpr void activateSSO() noexcept
		{
			if (std::is_constant_evaluated())
				for (size_t i = 0; i < SSOBufferSize; i++)
					m_data.buffer = value_type();
		}
		[[nodiscard]] constexpr size_type clampSuffixSize(const size_type offset, const size_type size) const noexcept
		{
			return std::min(size, m_size - offset);
		}
		[[nodiscard]] static constexpr size_type calculateGrow(const size_type requested, const size_type old, const size_type max) noexcept
		{
			const size_type masked = requested | _Alloc_mask;
			if (masked > max)
				return max;

			if (old > max - old / 2)
				return max;

			return std::max(masked, old + old / 2);
		}
		[[nodiscard]] constexpr size_type calculateGrow(const size_type requested)
		{
			return calculateGrow(requested, m_capacity, max_size());
		}
		constexpr void takeContents(basic_string& right) noexcept
		{
			// assign by stealing right's buffer
			// pre: this != &right
			// pre: allocator propagation (POCMA) from right, if necessary, is complete
			// pre: *this owns no memory, iterators orphaned
			// (note: _Buf/ptr/_Mysize/_Myres may be garbage init)

			if constexpr (_Can_memcpy_val) {
				if (!std::is_constant_evaluated())
				{
					const auto _My_data_mem = reinterpret_cast<unsigned char*>(std::addressof(*this));
					const auto _Right_data_mem = reinterpret_cast<const unsigned char*>(std::addressof(right));
					::memcpy(_My_data_mem, _Right_data_mem, sizeof(basic_string));

					right.m_size = 0;
					right.m_capacity = SSOCapacity;
					right.activateSSO();
					Traits::assign(right.m_data.buffer[0], value_type());
					return;
				}
			}

			if (right.isLargeString())
			{
				_Construct_in_place(m_data.ptr, right.m_data.ptr);
				right.m_data._Switch_to_buf();
			}
			else
			{
				activateSSO();
				Traits::copy(m_data.buffer, right.m_data.buffer, right.m_size + 1);
			}

			m_capacity = right.m_capacity;
			m_size = right.m_size;

			right.m_size = 0;
			right.m_capacity = SSOCapacity;
			Traits::assign(right.m_data.buffer[0], value_type());
		}

		constexpr void moveConstructFromSubstr(basic_string& right, const size_type offset, const size_type sizeMax) {
			const auto _Result_size = right.clampSuffixSize(offset, sizeMax);
			const auto _Right_ptr = right.data();
			if (alloc == right.alloc && _Result_size > SSOCapacity) {

				if (offset != 0) {
					Traits::move(_Right_ptr, _Right_ptr + offset, _Result_size);
				}
				right._Eos(_Result_size);
				takeContents(right);
			}
			else {
				construct<ConstructStrategy::FromPtr>(_Right_ptr + offset, _Result_size);
			}
		}
		constexpr void _Eos(const size_type newSize) noexcept
		{
			m_size = newSize;
			Traits::assign(data()[newSize], value_type());
		}

		constexpr void _Tidy_deallocate() noexcept
		{
			if (isLargeString()) {
				DeallocateForCapacity(alloc, m_data.ptr, m_capacity);
				m_data._Switch_to_buf();
			}

			m_size = 0;
			m_capacity = SSOCapacity;
			Traits::assign(m_data.buffer[0], value_type());
		}
		union
		{
			value_type buffer[SSOBufferSize];
			pointer ptr;
			constexpr void _Switch_to_buf() noexcept {
				std::_Destroy_in_place(ptr);
				if (std::is_constant_evaluated()) {
					for (size_type i = 0; i < SSOBufferSize; ++i)
						buffer[i] = value_type();
				}
			}
		} m_data;
		size_type m_size{};
		size_type m_capacity{};
		Allocator alloc{};
	};
	using string = basic_string<char>;
}