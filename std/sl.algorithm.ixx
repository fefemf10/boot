export module sl.algorithm;

export namespace std
{
	template<class T>
	constexpr const T& min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
	template<class T>
	constexpr const T& max(const T& a, const T& b)
	{
		return (a < b) ? b : a;
	}
}