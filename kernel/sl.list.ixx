export module sl.list;
import sl.typetraits;
import sl.numeric_limits;
import sl.memory;
import sl.iterator;
import sl.utility;
import sl.typetraits;
import sl.concepts;
import sl.type;
import sl.initializer_list;
export namespace std
{
	template <class T, class Allocator = allocator<T>>
	class list
	{
	private:
		class list_node;
	public:

		using value_type = T;
		using allocator_type = Allocator;
		using size_type = typename allocator_traits<Allocator>::size_type;
		using difference_type = typename allocator_traits<Allocator>::difference_type;
		using pointer = typename allocator_traits<Allocator>::pointer;
		using const_pointer = typename allocator_traits<Allocator>::const_pointer;
		using reference = T&;
		using const_reference = const T&;
		class iterator;

		//using reverse_iterator = reverse_iterator<iterator>;
		//using const_reverse_iterator = reverse_iterator<const_iterator>;

		constexpr list() noexcept(noexcept(Allocator())) { allocEmptyList(); }
		constexpr explicit list(const Allocator& alloc) noexcept { allocEmptyList(); }
		constexpr list(initializer_list<T> lst)
		{
			allocEmptyList();
			copy(lst.begin(), lst.end(), std::back_insert_iterator(*this));
		}
		constexpr list(iterator begin, iterator end);
		explicit list(size_type count);
		void assign(iterator first, iterator last);
	private:
		list_node* _head;
		list_node* _tail;
		size_type _size;
		Allocator alloc;
		constexpr list_node* getNode()
		{
			return alloc.allocate(1);
		}
		constexpr void allocEmptyList()
		{
			list_node* next = getNode();
			list_node* prev = getNode();
			prev->_prev = prev;
			prev->_next = next;
			next->_prev = prev;
			next->_next = next;
			_head = next;
			_tail = next;
			_size = 0;
		}
	};
	template <class T, class Allocator>
	class list<T, Allocator>::list_node
	{
	public:
		constexpr list_node() : _prev{}, _next{}, _value{} {}
		constexpr list_node(T value) : _value{ value } {}
		constexpr list_node(T value, list_node* prev, list_node* next) : _prev{ prev }, _next{ next }, _value{ value } {}
		constexpr list_node(list_node* prev, list_node* next) : _prev{ prev }, _next{ next } {}
	protected:
		friend class list;
		T _value;
		list_node* _prev;
		list_node* _next;
	};
	
	template <class T, class Allocator>
	class list<T, Allocator>::iterator
	{
	public:
		friend class list;
		using iterator_category = bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
		constexpr iterator() noexcept = default;
		constexpr iterator(list_node* node) noexcept : node(node) {}

		[[nodiscard]] constexpr reference operator*() const noexcept
		{
			return node->_value;
		}
		[[nodiscard]] constexpr pointer operator->() const noexcept
		{
			return &node->_value;
		}
		constexpr iterator& operator++() noexcept
		{
			node = node->_next;
			return *this;
		}
		constexpr iterator operator++(int) noexcept
		{
			iterator tmp = *this;
			++node;
			return tmp;
		}
		constexpr iterator& operator--() noexcept
		{
			node = node->_prev;
			return *this;
		}
		constexpr iterator operator--(int) noexcept
		{
			iterator tmp = *this;
			--node;
			return tmp;
		}
		constexpr iterator prev() const noexcept
		{
			iterator tmp = *this;
			--tmp;
			return tmp;
		}
		constexpr iterator next() const noexcept
		{
			iterator tmp = *this;
			++tmp;
			return tmp;
		}
		[[nodiscard]] constexpr bool operator==(const iterator& rhs) const noexcept
		{
			return node == rhs.node;
		}
	protected:
		list_node* node;
	};
}