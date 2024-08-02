export module sl.iterator;
import sl.memory;
import sl.compare;
import sl.utility;
import sl.type;
import sl.typetraits;
export namespace std
{
	template <class T>
	class back_insert_iterator
	{
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using pointer = void;
		using reference = void;

		using container_type = T;
		using difference_type = ptrdiff_t;
		constexpr explicit back_insert_iterator(T* cont) noexcept : container(addressof(cont)) {}
		constexpr back_insert_iterator& operator=(const typename T::value_type& value)
		{
			container->push_back(value);
			return *this;
		}
		constexpr back_insert_iterator& operator=(const typename T::value_type&& value)
		{
			//container->push_back(move(value));
			return *this;
		}
		[[nodiscard]] constexpr back_insert_iterator& operator*() noexcept
		{
			return *this;
		}
		constexpr back_insert_iterator& operator++() noexcept
		{
			return *this;
		}
		constexpr back_insert_iterator& operator++(int) noexcept
		{
			return *this;
		}
	protected:
		typename T::pointer ptr{};
		const T* container;
	};
	template <class T>
	class front_insert_iterator
	{
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using pointer = void;
		using reference = void;

		using container_type = T;
		using difference_type = ptrdiff_t;
		constexpr explicit front_insert_iterator(T* cont) noexcept : container(addressof(cont)) {}
		constexpr front_insert_iterator& operator=(const typename T::value_type& value)
		{
			container->push_front(value);
			return *this;
		}
		constexpr front_insert_iterator& operator=(const typename T::value_type&& value)
		{
			//container->push_front(move(value));
			return *this;
		}
		[[nodiscard]] constexpr front_insert_iterator& operator*() noexcept
		{
			return *this;
		}
		constexpr front_insert_iterator& operator++() noexcept
		{
			return *this;
		}
		constexpr front_insert_iterator& operator++(int) noexcept
		{
			return *this;
		}
	protected:
		T* container;
	};
	template <class T>
	class insert_iterator
	{
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using pointer = void;
		using reference = void;

		using container_type = T;
		using difference_type = ptrdiff_t;
		constexpr explicit insert_iterator(T& cont, T::iterator where) noexcept : container(addressof(cont)), iter(move(where)) {}
		constexpr insert_iterator& operator=(const typename T::value_type& value)
		{
			iter = container->insert(iter, value);
			++iter;
			return *this;
		}
		constexpr insert_iterator& operator=(const typename T::value_type&& value)
		{
			//iter = container->insert(iter, move(value));
			++iter;
			return *this;
		}
		[[nodiscard]] constexpr insert_iterator& operator*() noexcept
		{
			return *this;
		}
		constexpr insert_iterator& operator++() noexcept
		{
			return *this;
		}
		constexpr insert_iterator& operator++(int) noexcept
		{
			return *this;
		}
	protected:
		T* container;
		typename T::iterator iter;
	};
	template <class T>
	class const_linear_iterator
	{
	public:
		using iterator_category = random_access_iterator_tag;
		using value_type = typename T::value_type;
		using difference_type = typename T::difference_type;
		using pointer = typename T::const_pointer;
		using reference = const value_type&;

		constexpr const_linear_iterator() noexcept = default;
		constexpr const_linear_iterator(typename T::pointer ptr, const T* container) noexcept : ptr(ptr), container(const_cast<T*>(container)) {}
		[[nodiscard]] constexpr reference operator*() const noexcept
		{
			return *ptr;
		}
		[[nodiscard]] constexpr pointer operator->() const noexcept
		{
			return ptr;
		}
		constexpr const_linear_iterator& operator++() noexcept
		{
			++ptr;
			return *this;
		}
		constexpr const_linear_iterator& operator++(int) noexcept
		{
			const_linear_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		constexpr const_linear_iterator& operator--() noexcept
		{
			--ptr;
			return *this;
		}
		constexpr const_linear_iterator& operator--(int) noexcept
		{
			const_linear_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		constexpr const_linear_iterator& operator+=(const difference_type offset) noexcept
		{
			verify_offset(offset);
			ptr += offset;
			return *this;
		}
		[[nodiscard]] constexpr const_linear_iterator operator+(const difference_type offset) const noexcept
		{
			const_linear_iterator tmp = *this;
			tmp += offset;
			return tmp;
		}
		constexpr const_linear_iterator& operator-=(const difference_type offset) noexcept
		{
			verify_offset(-offset);
			ptr -= offset;
			return *this;
		}
		[[nodiscard]] constexpr const_linear_iterator operator-(const difference_type offset) const noexcept
		{
			const_linear_iterator tmp = *this;
			tmp -= offset;
			return tmp;
		}
		[[nodiscard]] constexpr reference operator[](const difference_type offset) const noexcept
		{
			return *(this + offset);
		}
		[[nodiscard]] constexpr bool operator==(const const_linear_iterator& rhs) const noexcept
		{
			return ptr == rhs.ptr;
		}
		[[nodiscard]] constexpr strong_ordering operator<=>(const const_linear_iterator& rhs) const noexcept
		{
			return ptr <=> rhs.ptr;
		}
	protected:
		constexpr void verify_offset(const difference_type offset) noexcept
		{
			static_assert(offset == 0 || ptr, "cannot seek value-initialized vector iterator");
			if constexpr (offset < 0)
			{
				static_assert(offset >= container->first - ptr, "cannot seek vector iterator before begin");
			}
			if constexpr (offset > 0)
			{
				static_assert(offset <= container->last - ptr, "cannot seek vector iterator after end");
			}
		}
		typename T::pointer ptr{};
		const T* container;
	};

	template <class T>
	class linear_iterator : public const_linear_iterator<T>
	{
	public:
		using iterator_category = random_access_iterator_tag;
		using value_type = typename T::value_type;
		using difference_type = typename T::difference_type;
		using pointer = typename T::pointer;
		using reference = value_type&;

		constexpr linear_iterator() noexcept = default;
		constexpr linear_iterator(typename T::pointer ptr, const T* container) noexcept : const_linear_iterator<T>(ptr, container) {}
		[[nodiscard]] constexpr reference operator*() const noexcept
		{
			return const_cast<reference>(base::operator*());
		}
		[[nodiscard]] constexpr pointer operator->() const noexcept
		{
			return this->ptr;
		}
		constexpr linear_iterator& operator++() noexcept
		{
			base::operator++();
			return *this;
		}
		constexpr linear_iterator& operator++(int) noexcept
		{
			linear_iterator tmp = *this;
			base::operator++();
			return tmp;
		}
		constexpr linear_iterator& operator--() noexcept
		{
			base::operator--();
			return *this;
		}
		constexpr linear_iterator& operator--(int) noexcept
		{
			linear_iterator tmp = *this;
			base::operator--();
			return tmp;
		}
		constexpr linear_iterator& operator+=(const difference_type offset) noexcept
		{
			verify_offset(offset);
			base::operator+=(offset);
			return *this;
		}
		[[nodiscard]] constexpr linear_iterator operator+(const difference_type offset) const noexcept
		{
			linear_iterator tmp = *this;
			tmp += offset;
			return tmp;
		}
		constexpr linear_iterator& operator-=(const difference_type offset) noexcept
		{
			verify_offset(-offset);
			base::operator-=(offset);
			return *this;
		}
		[[nodiscard]] constexpr linear_iterator operator-(const difference_type offset) const noexcept
		{
			linear_iterator tmp = *this;
			tmp -= offset;
			return tmp;
		}
		[[nodiscard]] constexpr reference operator[](const difference_type offset) const noexcept
		{
			return const_cast<reference>(base::operator[](offset));
		}
		[[nodiscard]] constexpr bool operator==(const linear_iterator& rhs) const noexcept
		{
			return this->ptr == rhs.ptr;
		}
		[[nodiscard]] constexpr strong_ordering operator<=>(const linear_iterator& rhs) const noexcept
		{
			return this->ptr <=> rhs.ptr;
		}
	private:
		using base = const_linear_iterator<T>;
	};

	template <class T>
	class reverse_iterator
	{
	public:
		using iterator_type = T;
		using iterator_concept = conditional_t<random_access_iterator<T>, random_access_iterator_tag, bidirectional_iterator_tag>;
		using iterator_category = conditional_t<derived_from<_Iter_cat_t<T>, random_access_iterator_tag>, random_access_iterator_tag, _Iter_cat_t<T>>;
		using value_type = _Iter_value_t<T>;
		using difference_type = _Iter_diff_t<T>;
		using pointer = typename iterator_traits<T>::pointer;
		using reference = _Iter_ref_t<T>;

		template <class>
		friend class reverse_iterator;

		constexpr reverse_iterator() = default;

		constexpr explicit reverse_iterator(T right) noexcept(is_nothrow_move_constructible_v<T>) : current(move(right)) {}
		template <class U> requires (!is_same_v<U, T>) && convertible_to<const U&, T>
		constexpr reverse_iterator(const reverse_iterator<U>& _Right) noexcept(is_nothrow_constructible_v<T, const U&>) : current(_Right.current) {}

		template <class U> requires (!is_same_v<U, T>) && convertible_to<const U&, T>&& assignable_from<T&, const U&>
		constexpr reverse_iterator& operator=(const reverse_iterator<U>& _Right) noexcept(is_nothrow_assignable_v<T&, const U&>)
		{
			current = _Right.current;
			return *this;
		}

		[[nodiscard]] constexpr T base() const noexcept(is_nothrow_copy_constructible_v<T>)
		{
			return current;
		}

		[[nodiscard]] constexpr reference operator*() const noexcept(is_nothrow_copy_constructible_v<T> && noexcept(*--(declval<T&>())))
		{
			T _Tmp = current;
			return *--_Tmp;
		}

		[[nodiscard]] constexpr pointer operator->() const noexcept(is_nothrow_copy_constructible_v<T> && noexcept(--(declval<T&>())) && _Has_nothrow_operator_arrow<T&, pointer>)
			requires (is_pointer_v<T> || requires(const T __i) { __i.operator->(); })
		{
			T _Tmp = current;
			--_Tmp;
			if constexpr (is_pointer_v<T>) {
				return _Tmp;
			}
			else {
				return _Tmp.operator->();
			}
		}

		constexpr reverse_iterator& operator++() noexcept(noexcept(--current))
		{
			--current;
			return *this;
		}

		constexpr reverse_iterator operator++(int) noexcept(is_nothrow_copy_constructible_v<T> && noexcept(--current))
		{
			reverse_iterator _Tmp = *this;
			--current;
			return _Tmp;
		}

		constexpr reverse_iterator& operator--() noexcept(noexcept(++current))
		{
			++current;
			return *this;
		}

		constexpr reverse_iterator operator--(int) noexcept(is_nothrow_copy_constructible_v<T> && noexcept(++current))
		{
			reverse_iterator _Tmp = *this;
			++current;
			return _Tmp;
		}

		[[nodiscard]] constexpr reverse_iterator operator+(const difference_type _Off) const noexcept(noexcept(reverse_iterator(current - _Off)))
		{
			return reverse_iterator(current - _Off);
		}

		constexpr reverse_iterator& operator+=(const difference_type _Off) noexcept(noexcept(current -= _Off))
		{
			current -= _Off;
			return *this;
		}

		[[nodiscard]] constexpr reverse_iterator operator-(const difference_type _Off) const noexcept(noexcept(reverse_iterator(current + _Off)))
		{
			return reverse_iterator(current + _Off);
		}

		constexpr reverse_iterator& operator-=(const difference_type _Off) noexcept(noexcept(current += _Off))
		{
			current += _Off;
			return *this;
		}

		[[nodiscard]] constexpr reference operator[](const difference_type _Off) const noexcept(noexcept(_Fake_copy_init<reference>(current[_Off])))
		{
			return current[static_cast<difference_type>(-_Off - 1)];
		}
	protected:
		T current;
	};
}