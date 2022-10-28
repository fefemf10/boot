export module std:initializer_list;

export namespace std
{
	template <class T>
	class initializer_list
	{
	public:
		using value_type = T;
		using reference = const T&;
		using const_reference = const T&;
		using size_type = size_t;

		using iterator = const T*;
		using const_iterator = const T*;

		constexpr initializer_list() noexcept = default;
		constexpr initializer_list(const T* first, const T* last) noexcept : first(first), last(last) {};
		constexpr const T* begin() const noexcept
		{
			return first;
		}
		constexpr const T* end() const noexcept
		{
			return last;
		}
		constexpr size_t size() const noexcept
		{
			return static_cast<size_t>(last - first);
		}
	private:
		const T* first{};
		const T* last{};
	};
	template <class T>
	[[nodiscard]] constexpr const T* begin(initializer_list<T> list) noexcept
	{
		return list.begin();
	}
	template <class T>
	[[nodiscard]] constexpr const T* end(initializer_list<T> list) noexcept
	{
		return list.end();
	}
}