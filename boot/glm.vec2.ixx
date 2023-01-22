export module glm.vec2;
export import glm.vec;
export namespace glm
{
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
		
		template<class U> constexpr explicit vec(const vec<1, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.x)) {}
		
		template<class A, class B> constexpr vec(A x, B y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}
		template<class A, class B> constexpr vec(const vec<1, A>& x, B y) : x(static_cast<T>(x.x)), y(static_cast<T>(y)) {}
		template<class A, class B> constexpr vec(A x, const vec<1, B>& y) : x(static_cast<T>(x)), y(static_cast<T>(y.x)) {}
		template<class A, class B> constexpr vec(const vec<1, A>& x, const vec<1, B>& y) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)) {}
		
		template<class U> constexpr explicit vec(const vec<2, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
		template<class U> constexpr explicit vec(const vec<3, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
		template<class U> constexpr explicit vec(const vec<4, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
		
		constexpr type& operator= (const type& v) { x = v.x; y = v.y; return *this; };
		template<class U> constexpr type& operator= (const vec<2, U>& v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); return *this; }
		
		template<class U> constexpr type& operator+= (U scalar) { x += static_cast<T>(scalar); y += static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator-= (U scalar) { x -= static_cast<T>(scalar); y -= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator*= (U scalar) { x *= static_cast<T>(scalar); y *= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator/= (U scalar) { x /= static_cast<T>(scalar); y /= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator+= (const vec<1, U>& v) { x += static_cast<T>(v.x); y += static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator-= (const vec<1, U>& v) { x -= static_cast<T>(v.x); y -= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator*= (const vec<1, U>& v) { x *= static_cast<T>(v.x); y *= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator/= (const vec<1, U>& v) { x /= static_cast<T>(v.x); y /= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator+= (const vec<2, U>& v) { x += static_cast<T>(v.x); y += static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator-= (const vec<2, U>& v) { x -= static_cast<T>(v.x); y -= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator*= (const vec<2, U>& v) { x *= static_cast<T>(v.x); y *= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator/= (const vec<2, U>& v) { x /= static_cast<T>(v.x); y /= static_cast<T>(v.y); return *this; }
		
		constexpr type& operator++() { ++x; ++y; return *this; }
		constexpr type& operator--() { --x; --y; return *this; }
		constexpr type operator++(int) { type result(*this); ++* this; return result; }
		constexpr type operator--(int) { type result(*this); --* this; return result; }
		
		template<class U> constexpr type& operator%= (U scalar) { x %= static_cast<T>(scalar); y %= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator&= (U scalar) { x &= static_cast<T>(scalar); y &= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator|= (U scalar) { x |= static_cast<T>(scalar); y |= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator^= (U scalar) { x ^= static_cast<T>(scalar); y ^= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator<<= (U scalar) { x <<= static_cast<T>(scalar); y <<= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator>>= (U scalar) { x >>= static_cast<T>(scalar); y >>= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator%= (const vec<1, U>& v) { x %= static_cast<T>(v.x); y %= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator&= (const vec<1, U>& v) { x &= static_cast<T>(v.x); y &= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator|= (const vec<1, U>& v) { x |= static_cast<T>(v.x); y |= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator^= (const vec<1, U>& v) { x ^= static_cast<T>(v.x); y ^= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator<<= (const vec<1, U>& v) { x <<= static_cast<T>(v.x); y <<= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator>>= (const vec<1, U>& v) { x >>= static_cast<T>(v.x); y >>= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator%= (const vec<2, U>& v) { x %= static_cast<T>(v.x); y %= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator&= (const vec<2, U>& v) { x &= static_cast<T>(v.x); y &= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator|= (const vec<2, U>& v) { x |= static_cast<T>(v.x); y |= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator^= (const vec<2, U>& v) { x ^= static_cast<T>(v.x); y ^= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator<<= (const vec<2, U>& v) { x <<= static_cast<T>(v.x); y <<= static_cast<T>(v.y); return *this; }
		template<class U> constexpr type& operator>>= (const vec<2, U>& v) { x >>= static_cast<T>(v.x); y >>= static_cast<T>(v.y); return *this; }
		
		constexpr type operator+ () { return *this; }
		constexpr type operator- () { return type(-x, -y); }
		
		constexpr type operator+ (const type& v) { return type(x + v.x, y + v.y); }
		constexpr type operator- (const type& v) { return type(x - v.x, y - v.y); }
		constexpr type operator* (const type& v) { return type(x * v.x, y * v.y); }
		constexpr type operator/ (const type& v) { return type(x / v.x, y / v.y); }
		constexpr type operator% (const type& v) { return type(x % v.x, y % v.y); }
		constexpr type operator& (const type& v) { return type(x & v.x, y & v.y); }
		constexpr type operator| (const type& v) { return type(x | v.x, y | v.y); }
		constexpr type operator^ (const type& v) { return type(x ^ v.x, y ^ v.y); }
		constexpr type operator<< (const type& v) { return type(x << v.x, y << v.y); }
		constexpr type operator>> (const type& v) { return type(x >> v.x, y >> v.y); }
		friend constexpr type operator+ (const type& v, T scalar) { return type(v.x + scalar, v.y + scalar); }
		friend constexpr type operator- (const type& v, T scalar) { return type(v.x - scalar, v.y - scalar); }
		friend constexpr type operator* (const type& v, T scalar) { return type(v.x * scalar, v.y * scalar); }
		friend constexpr type operator/ (const type& v, T scalar) { return type(v.x / scalar, v.y / scalar); }
		friend constexpr type operator% (const type& v, T scalar) { return type(v.x % scalar, v.y % scalar); }
		friend constexpr type operator& (const type& v, T scalar) { return type(v.x & scalar, v.y & scalar); }
		friend constexpr type operator| (const type& v, T scalar) { return type(v.x | scalar, v.y | scalar); }
		friend constexpr type operator^ (const type& v, T scalar) { return type(v.x ^ scalar, v.y ^ scalar); }
		friend constexpr type operator<< (const type& v, T scalar) { return type(v.x << scalar, v.y << scalar); }
		friend constexpr type operator>> (const type& v, T scalar) { return type(v.x >> scalar, v.y >> scalar); }
		friend constexpr type operator+ (T scalar, const type& v) { return type(scalar + v.x, scalar + v.y); }
		friend constexpr type operator- (T scalar, const type& v) { return type(scalar - v.x, scalar - v.y); }
		friend constexpr type operator* (T scalar, const type& v) { return type(scalar * v.x, scalar * v.y); }
		friend constexpr type operator/ (T scalar, const type& v) { return type(scalar / v.x, scalar / v.y); }
		friend constexpr type operator% (T scalar, const type& v) { return type(scalar % v.x, scalar % v.y); }
		friend constexpr type operator& (T scalar, const type& v) { return type(scalar & v.x, scalar & v.y); }
		friend constexpr type operator| (T scalar, const type& v) { return type(scalar | v.x, scalar | v.y); }
		friend constexpr type operator^ (T scalar, const type& v) { return type(scalar ^ v.x, scalar ^ v.y); }
		friend constexpr type operator<< (T scalar, const type& v) { return type(scalar << v.x, scalar << v.y); }
		friend constexpr type operator>> (T scalar, const type& v) { return type(scalar << v.x, scalar << v.y); }

		friend constexpr type operator+ (const type& v1, const vec<1, T>& v2) { return type(v1.x + v2.x, v1.y + v2.x); }
		friend constexpr type operator- (const type& v1, const vec<1, T>& v2) { return type(v1.x - v2.x, v1.y - v2.x); }
		friend constexpr type operator* (const type& v1, const vec<1, T>& v2) { return type(v1.x * v2.x, v1.y * v2.x); }
		friend constexpr type operator/ (const type& v1, const vec<1, T>& v2) { return type(v1.x / v2.x, v1.y / v2.x); }
		friend constexpr type operator% (const type& v1, const vec<1, T>& v2) { return type(v1.x % v2.x, v1.y % v2.x); }
		friend constexpr type operator& (const type& v1, const vec<1, T>& v2) { return type(v1.x & v2.x, v1.y & v2.x); }
		friend constexpr type operator| (const type& v1, const vec<1, T>& v2) { return type(v1.x | v2.x, v1.y | v2.x); }
		friend constexpr type operator^ (const type& v1, const vec<1, T>& v2) { return type(v1.x ^ v2.x, v1.y ^ v2.x); }
		friend constexpr type operator<< (const type& v1, const vec<1, T>& v2) { return type(v1.x << v2.x, v1.y << v2.x); }
		friend constexpr type operator>> (const type& v1, const vec<1, T>& v2) { return type(v1.x >> v2.x, v1.y >> v2.x); }
		friend constexpr type operator+ (const vec<1, T>& v1, const type& v2) { return type(v1.x + v2.x, v1.x + v2.y); }
		friend constexpr type operator- (const vec<1, T>& v1, const type& v2) { return type(v1.x - v2.x, v1.x - v2.y); }
		friend constexpr type operator* (const vec<1, T>& v1, const type& v2) { return type(v1.x * v2.x, v1.x * v2.y); }
		friend constexpr type operator/ (const vec<1, T>& v1, const type& v2) { return type(v1.x / v2.x, v1.x / v2.y); }
		friend constexpr type operator% (const vec<1, T>& v1, const type& v2) { return type(v1.x % v2.x, v1.x % v2.y); }
		friend constexpr type operator& (const vec<1, T>& v1, const type& v2) { return type(v1.x & v2.x, v1.x & v2.y); }
		friend constexpr type operator| (const vec<1, T>& v1, const type& v2) { return type(v1.x | v2.x, v1.x | v2.y); }
		friend constexpr type operator^ (const vec<1, T>& v1, const type& v2) { return type(v1.x ^ v2.x, v1.x ^ v2.y); }
		friend constexpr type operator<< (const vec<1, T>& v1, const type& v2) { return type(v1.x << v2.x, v1.x << v2.y); }
		friend constexpr type operator>> (const vec<1, T>& v1, const type& v2) { return type(v1.x >> v2.x, v1.x >> v2.y); }
		
		constexpr type operator~ () { return type(~x, ~y); }
		
		constexpr bool operator== (const type& v) { return x == v.x && y == v.y; }
		
		//friend constexpr bool_type operator&& (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x && v2.x, v1.y && v2.y); }
		//friend constexpr bool_type operator|| (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x || v2.x, v1.y || v2.y); }
		
		union
		{
			struct { T x, y; };
			struct { T r, g; };
			struct { T s, t; };
		};
	};
}