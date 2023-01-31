export module glm.geometric;
import glm.vec;
import sl.math;

export namespace glm
{
	template <class T>
	constexpr T length(T x)
	{
		return std::abs(x);
	}
	template <size_t L, class T>
	constexpr T length(const vec<L, T>& v)
	{
		return std::pow(dot(v, v), static_cast<T>(0.5));
	}
	template <class T>
	constexpr T distance(T a, T b)
	{
		return length(b - a);
	}
	template <size_t L, class T>
	constexpr T distance(const vec<L, T>& a, const vec<L, T>& b)
	{
		return length(b - a);
	}
	template <class T>
	constexpr T dot(T a, T b)
	{
		return a * b;
	}
	template <class T>
	constexpr T dot(const vec<1, T>& a, const vec<1, T>& b)
	{
		return a.x * b.x;
	}
	template <class T>
	constexpr T dot(const vec<2, T>& a, const vec<2, T>& b)
	{
		vec<2, T> tmp(a * b);
		return tmp.x + tmp.y;
	}
	template <class T>
	constexpr T dot(const vec<3, T>& a, const vec<3, T>& b)
	{
		vec<3, T> tmp(a * b);
		return tmp.x + tmp.y + tmp.z;
	}
	template <class T>
	constexpr T dot(const vec<4, T>& a, const vec<4, T>& b)
	{
		vec<4, T> tmp(a * b);
		return tmp.x + tmp.y + tmp.z + tmp.w;
	}
	template <size_t L, class T>
	constexpr T cross(const vec<3, T>& a, const vec<3, T>& b)
	{
		return vec<3, T>(
			a.y * b.z - b.y * a.z,
			a.z * b.x - b.z * a.x,
			a.x * b.y - b.x * a.y);
	}
	template <class T>
	constexpr const vec<3, T> normalize(const vec<3, T>& v)
	{
		return v / length(v);
	}
	template <size_t L, class T>
	constexpr vec<L, T> inversesqrt(const vec<L, T>& v)
	{
		return static_cast<T>(1) / sqrt(v);
	}
	template <class T>
	constexpr vec<1, T> sqrt(const vec<1, T>& v)
	{
		return pow(v, static_cast<T>(-0.5));
	}
	template <class T>
	constexpr vec<2, T> sqrt(const vec<2, T>& v)
	{
		return pow(v, static_cast<T>(-0.5));
	}
	template <class T>
	constexpr vec<3, T> sqrt(const vec<3, T>& v)
	{
		return pow(v, static_cast<T>(-0.5));
	}
	template <class T>
	constexpr vec<4, T> sqrt(const vec<4, T>& v)
	{
		return pow(v, static_cast<T>(-0.5));
	}
	template <class T>
	constexpr vec<1, T> pow(const vec<1, T>& v, T x)
	{
		return vec<1, T>(std::pow(v.x, x));
	}
	template <class T>
	constexpr vec<2, T> pow(const vec<2, T>& v, T x)
	{
		return vec<2, T>(std::pow(v.x, x), std::pow(v.y, x));
	}
	template <class T>
	constexpr vec<3, T> pow(const vec<3, T>& v, T x)
	{
		return vec<3, T>(std::pow(v.x, x), std::pow(v.y, x), std::pow(v.z, x));
	}
	template <class T>
	constexpr vec<4, T> pow(const vec<4, T>& v, T x)
	{
		return vec<4, T>(std::pow(v.x, x), std::pow(v.y, x), std::pow(v.z, x), std::pow(v.w, x));
	}
}