export module std:initializer_list;

export namespace std
{
	template <class T>
	struct initializer_list
	{
		constexpr initializer_list() noexcept = default;
		constexpr size_t size() const noexcept
		{
			return csize;
		}
		constexpr const T* begin() const noexcept
		{
			return cdata;
		}
		constexpr const T* end() const noexcept
		{
			return cdata + csize;
		}
		const T* cdata;
		size_t csize;
	};
}