export module std:vector;
import :allocator;
import :numeric_limits;
export namespace std
{
	template <class T, class Allocator = allocator<T>>
	class vector
	{
	public:
		constexpr vector() noexcept(noexcept(Allocator())) {};
		constexpr explicit vector(const Allocator& alloc) noexcept {};
		constexpr vector(const size_t count, const T& value, const Allocator& alloc = Allocator()) : csize(count), ccapacity(count)
		{
			cdata = alloc.allocate(count);
			for (size_t i = 0; i < count; ++i)
				cdata[i] = value;
		}
		constexpr explicit vector(const size_t count, const Allocator& alloc = Allocator()) : csize(count), ccapacity(count)
		{
			cdata = alloc.allocate(count);
		}
		constexpr vector(const vector& other) : vector(other, Allocator())
		{
		}
		constexpr vector(const vector& other, const Allocator& alloc)
		{
			cdata = alloc.allocate(other.ccapacity);
			for (size_t i = 0; i < other.csize; ++i)
				cdata[i] = other.cdata[i];
		}
		constexpr vector(vector&& other) noexcept;
		constexpr vector(vector&& other, const Allocator& alloc);
		[[nodiscard]] constexpr T* data() noexcept
		{
			return cdata;
		}
		[[nodiscard]] constexpr const T* data() const noexcept
		{
			return cdata;
		}
		[[nodiscard]] constexpr size_t size() const noexcept
		{
			return csize;
		}
		[[nodiscard]] constexpr size_t capacity() const noexcept
		{
			return ccapacity;
		}
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return !csize;
		}
		[[nodiscard]] constexpr size_t max_size() const noexcept
		{
			return numeric_limits<size_t>::max();
		}
		[[nodiscard]] constexpr void clear() noexcept
		{
			csize = 0;

		}
		[[nodiscard]] constexpr T& operator[](const size_t pos) noexcept
		{
				return cdata[pos];
		}
		[[nodiscard]] constexpr const T& operator[](const size_t pos) const noexcept
		{
			return cdata[pos];
		}
		[[nodiscard]] constexpr T& front(const size_t pos) noexcept
		{
			return cdata[0];
		}
		[[nodiscard]] constexpr const T& front(const size_t pos) const noexcept
		{
			return cdata[0];
		}
		[[nodiscard]] constexpr T& back(const size_t pos) noexcept
		{
			return cdata[csize-1];
		}
		[[nodiscard]] constexpr const T& back(const size_t pos) const noexcept
		{
			return cdata[csize-1];
		}
	private:
		constexpr size_t calculateGrow(const size_t newSize) const noexcept
		{
			if (ccapacity > max_size() >> 1)
				return max_size();
			const size_t geometric = ccapacity + ccapacity / 2;
			return (geometric < newSize) ? newSize : geometric;
		}
		T* cdata{};
		size_t csize{};
		size_t ccapacity{};
	};
}