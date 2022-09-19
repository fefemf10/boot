export module std:iterator;
import :memory;
import :compare;
import :typetraits;
import :concepts;
import :utility;
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
		T* container;
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
		constexpr const_linear_iterator(typename T::pointer ptr, const T* container) noexcept : ptr(ptr), container(container) {}
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
		T* container;
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
	};
}