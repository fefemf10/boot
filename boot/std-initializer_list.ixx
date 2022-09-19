export module std:initializer_list;

export namespace std
{
	template <class T>
	struct initializer_list
	{
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