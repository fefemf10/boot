export module std:vector;
import :allocator;
import :numeric_limits;
import :memory;
import :iterator;
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
		[[nodiscard]] constexpr T* data() noexcept
		{
			return m_first;
		}
		[[nodiscard]] constexpr const T* data() const noexcept
		{
			return m_first;
		}
		[[nodiscard]] constexpr size_t size() const noexcept
		{
			return static_cast<size_t>(m_last - m_first);
		}
		[[nodiscard]] constexpr size_t capacity() const noexcept
		{
			return static_cast<size_t>(m_end - m_first);
		}
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return m_first == m_last;
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
			return m_first[pos];
		}
		[[nodiscard]] constexpr const T& operator[](const size_t pos) const noexcept
		{
			return m_first[pos];
		}
		[[nodiscard]] constexpr iterator begin() noexcept
		{
			return iterator(m_first, addressof(*this));
		}
		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
			return const_iterator(m_first, addressof(*this));
		}
		[[nodiscard]] constexpr iterator end() noexcept
		{
			return iterator(m_last, addressof(*this));
		}
		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
			return const_iterator(m_last, addressof(*this));
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
		constexpr size_t calculateGrow(const size_t newSize) const noexcept
		{
			if (capacity() > max_size() >> 1)
				return max_size();
			const size_t geometric = capacity() + capacity() / 2;
			return (geometric < newSize) ? newSize : geometric;
		}
		T* m_first{};
		T* m_last{};
		T* m_end{};
	};
}