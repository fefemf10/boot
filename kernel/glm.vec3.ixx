export module glm.vec3;
export import glm.vec;
export namespace glm
{
	template<typename T>
	struct vec<3, T>
	{
		using value_type = T;
		using type = vec<3, T>;
		using bool_type = vec<3, bool>;

		constexpr static size_t length() { return 3; }
		T& operator[](size_t i) { switch (i) { default: case 0: return x; case 1: return y; case 2: return z; } }
		constexpr const T& operator[](size_t i) const { switch (i) { default: case 0: return x; case 1: return y; case 2: return z; } }

		constexpr vec() = default;
		constexpr vec(const type& v) = default;

		constexpr explicit vec(T scalar) : x(scalar), y(scalar), z(scalar) {}
		constexpr vec(T x, T y, T z) : x(x), y(y), z(z) {}

		template<class U> constexpr explicit vec(const vec<1, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.x)), z(static_cast<T>(v.x)) {}

		template<class X, class Y, class Z> constexpr vec(X x, Y y, Z z) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}
		template<class X, class Y, class Z> constexpr vec(X x, Y y, const vec<1, Z>& z) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)) {}
		template<class X, class Y, class Z> constexpr vec(X x, const vec<1, Y>& y, Z z) : x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)) {}
		template<class X, class Y, class Z> constexpr vec(X x, const vec<1, Y>& y, const vec<1, Z>& z) : x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)) {}
		template<class X, class Y, class Z> constexpr vec(const vec<1, X>& x, Y y, Z z) : x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}
		template<class X, class Y, class Z> constexpr vec(const vec<1, X>& x, Y y, const vec<1, Z>& z) : x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)) {}
		template<class X, class Y, class Z> constexpr vec(const vec<1, X>& x, const vec<1, Y>& y, Z z) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)) {}
		template<class X, class Y, class Z> constexpr vec(const vec<1, X>& x, const vec<1, Y>& y, const vec<1, Z>& z) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)) {}

		template<class A, class B> constexpr vec(const vec<2, A>& xy, B z) : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)) {}
		template<class A, class B> constexpr vec(const vec<2, A>& xy, const vec<1, B>& z) : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z.x)) {}
		template<class A, class B> constexpr vec(A x, const vec<2, B> yz) : x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)) {}
		template<class A, class B> constexpr vec(const vec<1, A>& x, const vec<2, B> yz) : x(static_cast<T>(x.x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)) {}
		
		template<class U> constexpr explicit vec(const vec<3, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}
		template<class U> constexpr explicit vec(const vec<4, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}
		
		constexpr type& operator= (const type& v) { x = v.x; y = v.y; z = v.z; return *this; };
		template<class U> constexpr type& operator= (const vec<3, U>& v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); z = static_cast<T>(v.z); return *this; }

		template<class U> constexpr type& operator+= (U scalar) { x += static_cast<T>(scalar); y += static_cast<T>(scalar); z += static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator-= (U scalar) { x -= static_cast<T>(scalar); y -= static_cast<T>(scalar); z -= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator*= (U scalar) { x *= static_cast<T>(scalar); y *= static_cast<T>(scalar); z *= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator/= (U scalar) { x /= static_cast<T>(scalar); y /= static_cast<T>(scalar); z /= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator+= (const vec<3, U>& v) { x += static_cast<T>(v.x); y += static_cast<T>(v.y); z += static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator-= (const vec<3, U>& v) { x -= static_cast<T>(v.x); y -= static_cast<T>(v.y); z -= static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator*= (const vec<3, U>& v) { x *= static_cast<T>(v.x); y *= static_cast<T>(v.y); z *= static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator/= (const vec<3, U>& v) { x /= static_cast<T>(v.x); y /= static_cast<T>(v.y); z /= static_cast<T>(v.z); return *this; }

		constexpr type& operator++() { ++x; ++y; ++z; return *this; }
		constexpr type& operator--() { --x; --y; --z; return *this; }
		constexpr type operator++(int) { type result(*this); ++* this; return result; }
		constexpr type operator--(int) { type result(*this); --* this; return result; }

		template<class U> constexpr type& operator%= (U scalar) { x %= static_cast<T>(scalar); y %= static_cast<T>(scalar); z %= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator&= (U scalar) { x &= static_cast<T>(scalar); y &= static_cast<T>(scalar); z &= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator|= (U scalar) { x |= static_cast<T>(scalar); y |= static_cast<T>(scalar); z |= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator^= (U scalar) { x ^= static_cast<T>(scalar); y ^= static_cast<T>(scalar); z ^= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator<<= (U scalar) { x <<= static_cast<T>(scalar); y <<= static_cast<T>(scalar); z <<= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator>>= (U scalar) { x >>= static_cast<T>(scalar); y >>= static_cast<T>(scalar); z >>= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator%= (const vec<3, U>& v) { x %= static_cast<T>(v.x); y %= static_cast<T>(v.y); z %= static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator&= (const vec<3, U>& v) { x &= static_cast<T>(v.x); y &= static_cast<T>(v.y); z &= static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator|= (const vec<3, U>& v) { x |= static_cast<T>(v.x); y |= static_cast<T>(v.y); z |= static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator^= (const vec<3, U>& v) { x ^= static_cast<T>(v.x); y ^= static_cast<T>(v.y); z ^= static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator<<= (const vec<3, U>& v) { x <<= static_cast<T>(v.x); y <<= static_cast<T>(v.y); z <<= static_cast<T>(v.z); return *this; }
		template<class U> constexpr type& operator>>= (const vec<3, U>& v) { x >>= static_cast<T>(v.x); y >>= static_cast<T>(v.y); z >>= static_cast<T>(v.z); return *this; }

		constexpr type operator+ () { return *this; }
		constexpr type operator- () { return type(-x, -y, -z); }

		constexpr type operator+ (const type& v) const { return type(x + v.x, y + v.y, z + v.z); }
		constexpr type operator- (const type& v) const { return type(x - v.x, y - v.y, z - v.z); }
		constexpr type operator* (const type& v) const { return type(x * v.x, y * v.y, z * v.z); }
		constexpr type operator/ (const type& v) const { return type(x / v.x, y / v.y, z / v.z); }
		constexpr type operator% (const type& v) const { return type(x % v.x, y % v.y, z % v.z); }
		constexpr type operator& (const type& v) const { return type(x & v.x, y & v.y, z & v.z); }
		constexpr type operator| (const type& v) const { return type(x | v.x, y | v.y, z | v.z); }
		constexpr type operator^ (const type& v) const { return type(x ^ v.x, y ^ v.y, z ^ v.z); }
		constexpr type operator<< (const type& v) const { return type(x << v.x, y << v.y, z << v.z); }
		constexpr type operator>> (const type& v) const { return type(x >> v.x, y >> v.y, z >> v.z); }
		friend constexpr type operator+ (const type& v, T scalar) { return type(v.x + scalar, v.y + scalar, v.z + scalar); }
		friend constexpr type operator- (const type& v, T scalar) { return type(v.x - scalar, v.y - scalar, v.z - scalar); }
		friend constexpr type operator* (const type& v, T scalar) { return type(v.x * scalar, v.y * scalar, v.z * scalar); }
		friend constexpr type operator/ (const type& v, T scalar) { return type(v.x / scalar, v.y / scalar, v.z / scalar); }
		friend constexpr type operator% (const type& v, T scalar) { return type(v.x % scalar, v.y % scalar, v.z % scalar); }
		friend constexpr type operator& (const type& v, T scalar) { return type(v.x & scalar, v.y & scalar, v.z & scalar); }
		friend constexpr type operator| (const type& v, T scalar) { return type(v.x | scalar, v.y | scalar, v.z | scalar); }
		friend constexpr type operator^ (const type& v, T scalar) { return type(v.x ^ scalar, v.y ^ scalar, v.z ^ scalar); }
		friend constexpr type operator<< (const type& v, T scalar) { return type(v.x << scalar, v.y << scalar, v.z << scalar); }
		friend constexpr type operator>> (const type& v, T scalar) { return type(v.x >> scalar, v.y >> scalar, v.z >> scalar); }
		friend constexpr type operator+ (T scalar, const type& v) { return type(scalar + v.x, scalar + v.y, scalar + v.z); }
		friend constexpr type operator- (T scalar, const type& v) { return type(scalar - v.x, scalar - v.y, scalar - v.z); }
		friend constexpr type operator* (T scalar, const type& v) { return type(scalar * v.x, scalar * v.y, scalar * v.z); }
		friend constexpr type operator/ (T scalar, const type& v) { return type(scalar / v.x, scalar / v.y, scalar / v.z); }
		friend constexpr type operator% (T scalar, const type& v) { return type(scalar % v.x, scalar % v.y, scalar % v.z); }
		friend constexpr type operator& (T scalar, const type& v) { return type(scalar & v.x, scalar & v.y, scalar & v.z); }
		friend constexpr type operator| (T scalar, const type& v) { return type(scalar | v.x, scalar | v.y, scalar | v.z); }
		friend constexpr type operator^ (T scalar, const type& v) { return type(scalar ^ v.x, scalar ^ v.y, scalar ^ v.z); }
		friend constexpr type operator<< (T scalar, const type& v) { return type(scalar << v.x, scalar << v.y, scalar << v.z); }
		friend constexpr type operator>> (T scalar, const type& v) { return type(scalar >> v.x, scalar >> v.y, scalar >> v.z); }

		friend constexpr type operator+ (const type& v1, const vec<1, T>& v2) { return type(v1.x + v2.x, v1.y + v2.x, v1.z + v2.x); }
		friend constexpr type operator- (const type& v1, const vec<1, T>& v2) { return type(v1.x - v2.x, v1.y - v2.x, v1.z - v2.x); }
		friend constexpr type operator* (const type& v1, const vec<1, T>& v2) { return type(v1.x * v2.x, v1.y * v2.x, v1.z * v2.x); }
		friend constexpr type operator/ (const type& v1, const vec<1, T>& v2) { return type(v1.x / v2.x, v1.y / v2.x, v1.z / v2.x); }
		friend constexpr type operator% (const type& v1, const vec<1, T>& v2) { return type(v1.x % v2.x, v1.y % v2.x, v1.z % v2.x); }
		friend constexpr type operator& (const type& v1, const vec<1, T>& v2) { return type(v1.x & v2.x, v1.y & v2.x, v1.z & v2.x); }
		friend constexpr type operator| (const type& v1, const vec<1, T>& v2) { return type(v1.x | v2.x, v1.y | v2.x, v1.z | v2.x); }
		friend constexpr type operator^ (const type& v1, const vec<1, T>& v2) { return type(v1.x ^ v2.x, v1.y ^ v2.x, v1.z ^ v2.x); }
		friend constexpr type operator<< (const type& v1, const vec<1, T>& v2) { return type(v1.x << v2.x, v1.y << v2.x, v1.z << v2.x); }
		friend constexpr type operator>> (const type& v1, const vec<1, T>& v2) { return type(v1.x >> v2.x, v1.y >> v2.x, v1.z >> v2.x); }
		friend constexpr type operator+ (const vec<1, T>& v1, const type& v2) { return type(v1.x + v2.x, v1.x + v2.y, v1.x + v2.z); }
		friend constexpr type operator- (const vec<1, T>& v1, const type& v2) { return type(v1.x - v2.x, v1.x - v2.y, v1.x - v2.z); }
		friend constexpr type operator* (const vec<1, T>& v1, const type& v2) { return type(v1.x * v2.x, v1.x * v2.y, v1.x * v2.z); }
		friend constexpr type operator/ (const vec<1, T>& v1, const type& v2) { return type(v1.x / v2.x, v1.x / v2.y, v1.x / v2.z); }
		friend constexpr type operator% (const vec<1, T>& v1, const type& v2) { return type(v1.x % v2.x, v1.x % v2.y, v1.x % v2.z); }
		friend constexpr type operator& (const vec<1, T>& v1, const type& v2) { return type(v1.x & v2.x, v1.x & v2.y, v1.x & v2.z); }
		friend constexpr type operator| (const vec<1, T>& v1, const type& v2) { return type(v1.x | v2.x, v1.x | v2.y, v1.x | v2.z); }
		friend constexpr type operator^ (const vec<1, T>& v1, const type& v2) { return type(v1.x ^ v2.x, v1.x ^ v2.y, v1.x ^ v2.z); }
		friend constexpr type operator<< (const vec<1, T>& v1, const type& v2) { return type(v1.x << v2.x, v1.x << v2.y, v1.x << v2.z); }
		friend constexpr type operator>> (const vec<1, T>& v1, const type& v2) { return type(v1.x >> v2.x, v1.x >> v2.y, v1.x >> v2.z); }

		constexpr type operator~ () { return type(~x, ~y, ~z); }

		constexpr bool operator== (const type& v) { return x == v.x && y == v.y && z == v.z; }

		//friend constexpr bool_type operator&& (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x && v2.x, v1.y && v2.y); }
		//friend constexpr bool_type operator|| (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x || v2.x, v1.y || v2.y); }

		union
		{
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T s, t, p; };
		};
	};
}