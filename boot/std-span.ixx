export module std:span;
import :type;
export namespace std
{
	template <class T>
	class span
	{
		T* data{};
		size_t size{};
	};
}