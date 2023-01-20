export module glm.vec;
import types;

export namespace glm
{
	template<size_t L, typename T> struct vec;
	using f32vec2 = vec<2, f32>;
	using f32vec3 = vec<3, f32>;
	using f32vec4 = vec<4, f32>;
	using f64vec2 = vec<2, f64>;
	using f64vec3 = vec<3, f64>;
	using f64vec4 = vec<4, f64>;
	using i8vec2 = vec<2, i8>;
	using i8vec3 = vec<3, i8>;
	using i8vec4 = vec<4, i8>;
	using u8vec2 = vec<2, u8>;
	using u8vec3 = vec<3, u8>;
	using u8vec4 = vec<4, u8>;
	using i16vec2 = vec<2, i16>;
	using i16vec3 = vec<3, i16>;
	using i16vec4 = vec<4, i16>;
	using u16vec2 = vec<2, u16>;
	using u16vec3 = vec<3, u16>;
	using u16vec4 = vec<4, u16>;
	using i32vec2 = vec<2, i32>;
	using i32vec3 = vec<3, i32>;
	using i32vec4 = vec<4, i32>;
	using u32vec2 = vec<2, u32>;
	using u32vec3 = vec<3, u32>;
	using u32vec4 = vec<4, u32>;
	using i64vec2 = vec<2, i64>;
	using i64vec3 = vec<3, i64>;
	using i64vec4 = vec<4, i64>;
	using u64vec2 = vec<2, u64>;
	using u64vec3 = vec<3, u64>;
	using u64vec4 = vec<4, u64>;

	template<typename T>
	struct vec<2, T>
	{
		using value_type = T;
		using type = vec<2, T>;
		using bool_type = vec<2, bool>;

		constexpr static size_t length() { return 2; }
		T& operator[](size_t i) { switch (i) { default: case 0: return x; case 1: return y; } }
		constexpr const T& operator[](size_t i) const { switch (i) { default: case 0: return x; case 1: return y; } }

		constexpr vec() = default;
		constexpr vec(const type& v) = default;

		constexpr explicit vec(T scalar) : x(scalar), y(scalar) {}
		constexpr vec(T x, T y) : x(x), y(y) {}
		
		template<class A, class B> constexpr vec(A x, B y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

		template<class U> constexpr explicit vec(const vec<2, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
		template<class U> constexpr explicit vec(const vec<3, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
		template<class U> constexpr explicit vec(const vec<4, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
		
		constexpr type& operator= (const type& v) { x = v.x; y = v.y; return *this; };
		template<class U> constexpr type& operator= (const vec<2, U>& v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); return *this; }

		template<class U> constexpr type& operator+= (U scalar) { x += static_cast<T>(scalar); y += static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator+= (const vec<2, U>& v) { x += static_cast<T>(v.x); y += static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator-= (U scalar) { x -= static_cast<T>(scalar); y -= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator-= (const vec<2, U>& v) { x -= static_cast<T>(v.x); y -= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator*= (U scalar) { x *= static_cast<T>(scalar); y *= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator*= (const vec<2, U>& v) { x *= static_cast<T>(v.x); y *= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator/= (U scalar) { x /= static_cast<T>(scalar); y /= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator/= (const vec<2, U>& v) { x /= static_cast<T>(v.x); y /= static_cast<T>(v.y); return *this; }
		
		constexpr type& operator++() { ++x; ++y; return *this; };
		constexpr type& operator--() { --x; --y; return *this; };
		constexpr type operator++(int) { type result(*this); ++* this; return result; };
		constexpr type operator--(int) { type result(*this); --* this; return result; };
		
		template<class U> constexpr type& operator%= (U scalar) { x %= static_cast<T>(scalar); y %= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator%= (const vec<2, U>& v) { x %= static_cast<T>(v.x); y %= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator&= (U scalar){ x &= static_cast<T>(scalar); y &= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator&= (const vec<2, U>& v) { x &= static_cast<T>(v.x); y &= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator|= (U scalar) { x |= static_cast<T>(scalar); y |= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator|= (const vec<2, U>& v) { x |= static_cast<T>(v.x); y |= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator^= (U scalar) { x ^= static_cast<T>(scalar); y ^= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator^= (const vec<2, U>& v) { x ^= static_cast<T>(v.x); y ^= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator<<= (U scalar) { x <<= static_cast<T>(scalar); y <<= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator<<= (const vec<2, U>& v) { x <<= static_cast<T>(v.x); y <<= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator>>= (U scalar) { x >>= static_cast<T>(scalar); y >>= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator>>= (const vec<2, U>& v) { x >>= static_cast<T>(v.x); y >>= static_cast<T>(v.y); return *this; }

		constexpr type operator+ () { return *this; };
		constexpr type operator- () { return type(-x, -y); };
		
		friend constexpr type operator+ (const type& v, T scalar) { return type(v.x + scalar, v.y + scalar); }
		friend constexpr type operator+ (T scalar, const type& v) { return type(scalar + v.x, scalar + v.y); }
		constexpr type operator+ (const type& v) { return type(x + v.x, y + v.y); }
		
		friend constexpr type operator- (const type& v, T scalar) { return type(v.x - scalar, v.y - scalar); }
		friend constexpr type operator- (T scalar, const type& v) { return type(scalar - v.x, scalar - v.y); }
		constexpr type operator- (const type& v) { return type(x - v.x, y - v.y); }

		friend constexpr type operator* (const type& v, T scalar) { return type(v.x * scalar, v.y * scalar); }
		friend constexpr type operator* (T scalar, const type& v) { return type(scalar * v.x, scalar * v.y); }
		constexpr type operator* (const type& v) { return type(x * v.x, y * v.y); }

		friend constexpr type operator/ (const type& v, T scalar) { return type(v.x / scalar, v.y / scalar); }
		friend constexpr type operator/ (T scalar, const type& v) { return type(scalar / v.x, scalar / v.y); }
		constexpr type operator/ (const type& v) { return type(x / v.x, y / v.y); }

		friend constexpr type operator% (const type& v, T scalar) { return type(v.x % scalar, v.y % scalar); }
		friend constexpr type operator% (T scalar, const type& v) { return type(scalar % v.x, scalar % v.y); }
		constexpr type operator% (const type& v) { return type(x % v.x, y % v.y); }

		friend constexpr type operator& (const type& v, T scalar) { return type(v.x & scalar, v.y & scalar); }
		friend constexpr type operator& (T scalar, const type& v) { return type(scalar & v.x, scalar & v.y); }
		constexpr type operator& (const type& v) { return type(x & v.x, y & v.y); }

		friend constexpr type operator| (const type& v, T scalar) { return type(v.x | scalar, v.y | scalar); }
		friend constexpr type operator| (T scalar, const type& v) { return type(scalar | v.x, scalar | v.y); }
		constexpr type operator| (const type& v) { return type(x | v.x, y | v.y); }

		friend constexpr type operator^ (const type& v, T scalar) { return type(v.x ^ scalar, v.y ^ scalar); }
		friend constexpr type operator^ (T scalar, const type& v) { return type(scalar ^ v.x, scalar ^ v.y); }
		constexpr type operator^ (const type& v) { return type(x ^ v.x, y ^ v.y); }

		friend constexpr type operator<< (const type& v, T scalar) { return type(v.x << scalar, v.y << scalar); }
		friend constexpr type operator<< (T scalar, const type& v) { return type(scalar << v.x, scalar << v.y); }
		constexpr type operator<< (const type& v) { return type(x << v.x, y << v.y); }

		friend constexpr type operator>> (const type& v, T scalar) { return type(v.x >> scalar, v.y >> scalar); }
		friend constexpr type operator>> (T scalar, const type& v) { return type(scalar >> v.x, scalar >> v.y); }
		constexpr type operator>> (const type& v) { return type(x >> v.x, y >> v.y); }

		constexpr type operator~ () { return type(~x, ~y); }
		
		constexpr bool operator== (const type& v) { return x == v.x && y == v.y; }

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
		using value_type = T;
		using type = vec<3, T>;
		using bool_type = vec<3, bool>;

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
		using value_type = T;
		using type = vec<4, T>;
		using bool_type = vec<4, bool>;

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
}