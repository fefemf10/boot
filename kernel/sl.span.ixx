export module sl.span;
import sl.type;
export namespace std
{
	template <class T>
	class span
	{
		T* data{};
		size_t size{};
	};
}