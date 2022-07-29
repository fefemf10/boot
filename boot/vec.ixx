export module vec;
import types;
extern "C" int _fltused = 0;
export template<size_t L, typename T> struct vec;
export using f32vec2 = vec<2, f32>;
export using f32vec3 = vec<3, f32>;
export using f32vec4 = vec<4, f32>;
export using f64vec2 = vec<2, f64>;
export using f64vec3 = vec<3, f64>;
export using f64vec4 = vec<4, f64>;
export using i8vec2 = vec<2, i8>;
export using i8vec3 = vec<3, i8>;
export using i8vec4 = vec<4, i8>;
export using u8vec2 = vec<2, u8>;
export using u8vec3 = vec<3, u8>;
export using u8vec4 = vec<4, u8>;
export using i16vec2 = vec<2, i16>;
export using i16vec3 = vec<3, i16>;
export using i16vec4 = vec<4, i16>;
export using u16vec2 = vec<2, u16>;
export using u16vec3 = vec<3, u16>;
export using u16vec4 = vec<4, u16>;
export using i32vec2 = vec<2, i32>;
export using i32vec3 = vec<3, i32>;
export using i32vec4 = vec<4, i32>;
export using u32vec2 = vec<2, u32>;
export using u32vec3 = vec<3, u32>;
export using u32vec4 = vec<4, u32>;
export using i64vec2 = vec<2, i64>;
export using i64vec3 = vec<3, i64>;
export using i64vec4 = vec<4, i64>;
export using u64vec2 = vec<2, u64>;
export using u64vec3 = vec<3, u64>;
export using u64vec4 = vec<4, u64>;

template<typename T>
struct vec<2, T>
{
	vec() = default;
	vec(T a) : x(a), y(a) {}
	vec(T a, T b) : x(a), y(b) {}
	union
	{
		struct { T x, y; };
		struct { T r, g; };
		struct { T s, t; };
	};
};

template<typename T>
struct vec<3, T>
{
	vec() = default;
	vec(T a) : x(a), y(a), z(a) {}
	vec(T a, T b, T c) : x(a), y(b), z(c) {}
	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T s, t, p; };
	};
};
template<typename T>
struct vec<4, T>
{
	vec() = default;
	vec(T a) : x(a), y(a), z(a), w(a) {}
	vec(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {}
	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T s, t, p, q; };
	};
};