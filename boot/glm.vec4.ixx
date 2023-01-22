export module glm.vec4;
export import glm.vec;
export namespace glm
{
	template<typename T>
	struct vec<4, T>
	{
		using value_type = T;
		using type = vec<4, T>;
		using bool_type = vec<4, bool>;

		constexpr static size_t length() { return 4; }
		T& operator[](size_t i) { switch (i) { default: case 0: return x; case 1: return y; case 2: return z; case 3: return w; } }
		constexpr const T& operator[](size_t i) const { switch (i) { default: case 0: return x; case 1: return y; case 2: return z; case 3: return w; } }

		constexpr vec() = default;
		constexpr vec(const type& v) = default;
		
		constexpr explicit vec(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
		constexpr vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		
		template<class U> constexpr explicit vec(const vec<1, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.x)), z(static_cast<T>(v.x)), w(static_cast<T>(v.x)) {}

		template<class X, class Y, class Z, class W> constexpr vec(X x, Y y, Z z, W w) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(X x, Y y, Z z, const vec<1, W>& w) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}
		template<class X, class Y, class Z, class W> constexpr vec(X x, Y y, const vec<1, Z>& z, W w) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(X x, Y y, const vec<1, Z>& z, const vec<1, W>& w) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}
		template<class X, class Y, class Z, class W> constexpr vec(X x, const vec<1, Y>& y, Z z, W w) : x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(X x, const vec<1, Y>& y, Z z, const vec<1, W>& w) : x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}
		template<class X, class Y, class Z, class W> constexpr vec(X x, const vec<1, Y>& y, const vec<1, Z>& z, W w) : x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(X x, const vec<1, Y>& y, const vec<1, Z>& z, const vec<1, W>& w) : x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, Y y, Z z, W w) : x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, Y y, Z z, const vec<1, W>& w) : x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, Y y, const vec<1, Z>& z, W w) : x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, Y y, const vec<1, Z>& z, const vec<1, W>& w) : x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, const vec<1, Y>& y, Z z, W w) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, const vec<1, Y>& y, Z z, const vec<1, W>& w) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, const vec<1, Y>& y, const vec<1, Z>& z, W w) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}
		template<class X, class Y, class Z, class W> constexpr vec(const vec<1, X>& x, const vec<1, Y>& y, const vec<1, Z>& z, const vec<1, W>& w) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}
		
		template<class A, class B, class C> constexpr vec(const vec<2, A>& xy, B z, C w) : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}
		template<class A, class B, class C> constexpr vec(const vec<2, A>& xy, B z, const vec<1, C>& w) : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}
		template<class A, class B, class C> constexpr vec(const vec<2, A>& xy, const vec<1, B>& z, C w) : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}
		template<class A, class B, class C> constexpr vec(const vec<2, A>& xy, const vec<1, B>& z, const vec<1, C>& w) : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}
		template<class A, class B, class C> constexpr vec(A x, const vec<2, B>& yz, C w) : x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w)) {}
		template<class A, class B, class C> constexpr vec(A x, const vec<2, B>& yz, const vec<1, C>& w) : x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w.x)) {}
		template<class A, class B, class C> constexpr vec(const vec<1, A>& x, const vec<2, B>& yz, C w) : x(static_cast<T>(x.x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w)) {}
		template<class A, class B, class C> constexpr vec(const vec<1, A>& x, const vec<2, B>& yz, const vec<1, C>& w) : x(static_cast<T>(x.x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w.x)) {}
		template<class A, class B, class C> constexpr vec(A x, B y, const vec<2, C>& zw) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}
		template<class A, class B, class C> constexpr vec(A x, const vec<1, B>& y, const vec<2, C>& zw) : x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}
		template<class A, class B, class C> constexpr vec(const vec<1, A>& x, B y, const vec<2, C>& zw) : x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}
		template<class A, class B, class C> constexpr vec(const vec<1, A>& x, const vec<1, B>& y, const vec<2, C>& zw) : x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}
		template<class A, class B> constexpr vec(const vec<3, A>& xyz, B w) : x(static_cast<T>(xyz.x)), y(static_cast<T>(xyz.y)), z(static_cast<T>(xyz.z)), w(static_cast<T>(w)) {}
		template<class A, class B> constexpr vec(const vec<3, A>& xyz, const vec<1, B>& w) : x(static_cast<T>(xyz.x)), y(static_cast<T>(xyz.y)), z(static_cast<T>(xyz.z)), w(static_cast<T>(w.x)) {}
		template<class A, class B> constexpr vec(A x, const vec<3, B>& yzw) : x(static_cast<T>(x)), y(static_cast<T>(yzw.x)), z(static_cast<T>(yzw.y)), w(static_cast<T>(yzw.z)) {}
		template<class A, class B> constexpr vec(const vec<1, A>& x, const vec<3, B>& yzw) : x(static_cast<T>(x.x)), y(static_cast<T>(yzw.x)), z(static_cast<T>(yzw.y)), w(static_cast<T>(yzw.z)) {}
		template<class A, class B> constexpr vec(const vec<2, A>& xy, const vec<2, B>& zw) : x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

		template<class U> constexpr explicit vec(const vec<4, U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}
		
		constexpr type& operator= (const type& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; };
		template<class U> constexpr type& operator= (const vec<4, U>& v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); z = static_cast<T>(v.z); w = static_cast<T>(v.w); return *this; }
		
		template<class U> constexpr type& operator+= (U scalar) { x += static_cast<T>(scalar); y += static_cast<T>(scalar); z += static_cast<T>(scalar); w += static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator-= (U scalar) { x -= static_cast<T>(scalar); y -= static_cast<T>(scalar); z -= static_cast<T>(scalar); w -= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator*= (U scalar) { x *= static_cast<T>(scalar); y *= static_cast<T>(scalar); z *= static_cast<T>(scalar); w *= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator/= (U scalar) { x /= static_cast<T>(scalar); y /= static_cast<T>(scalar); z /= static_cast<T>(scalar); w /= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator+= (const vec<4, U>& v) { x += static_cast<T>(v.x); y += static_cast<T>(v.y); z += static_cast<T>(v.z); w += static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator-= (const vec<4, U>& v) { x -= static_cast<T>(v.x); y -= static_cast<T>(v.y); z -= static_cast<T>(v.z); w -= static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator*= (const vec<4, U>& v) { x *= static_cast<T>(v.x); y *= static_cast<T>(v.y); z *= static_cast<T>(v.z); w *= static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator/= (const vec<4, U>& v) { x /= static_cast<T>(v.x); y /= static_cast<T>(v.y); z /= static_cast<T>(v.z); w /= static_cast<T>(v.w); return *this; }
		
		constexpr type& operator++() { ++x; ++y; ++z; ++w;  return *this; }
		constexpr type& operator--() { --x; --y; --z; --w; return *this; }
		constexpr type operator++(int) { type result(*this); ++* this; return result; }
		constexpr type operator--(int) { type result(*this); --* this; return result; }
		
		template<class U> constexpr type& operator%= (U scalar) { x %= static_cast<T>(scalar); y %= static_cast<T>(scalar); z %= static_cast<T>(scalar); w %= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator&= (U scalar) { x &= static_cast<T>(scalar); y &= static_cast<T>(scalar); z &= static_cast<T>(scalar); w &= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator|= (U scalar) { x |= static_cast<T>(scalar); y |= static_cast<T>(scalar); z |= static_cast<T>(scalar); w |= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator^= (U scalar) { x ^= static_cast<T>(scalar); y ^= static_cast<T>(scalar); z ^= static_cast<T>(scalar); w ^= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator<<= (U scalar) { x <<= static_cast<T>(scalar); y <<= static_cast<T>(scalar); z <<= static_cast<T>(scalar); w <<= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator>>= (U scalar) { x >>= static_cast<T>(scalar); y >>= static_cast<T>(scalar); z >>= static_cast<T>(scalar); w >>= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator%= (const vec<4, U>& v) { x %= static_cast<T>(v.x); y %= static_cast<T>(v.y); z %= static_cast<T>(v.z); w %= static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator&= (const vec<4, U>& v) { x &= static_cast<T>(v.x); y &= static_cast<T>(v.y); z &= static_cast<T>(v.z); w &= static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator|= (const vec<4, U>& v) { x |= static_cast<T>(v.x); y |= static_cast<T>(v.y); z |= static_cast<T>(v.z); w |= static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator^= (const vec<4, U>& v) { x ^= static_cast<T>(v.x); y ^= static_cast<T>(v.y); z ^= static_cast<T>(v.z); w ^= static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator<<= (const vec<4, U>& v) { x <<= static_cast<T>(v.x); y <<= static_cast<T>(v.y); z <<= static_cast<T>(v.z); w <<= static_cast<T>(v.w); return *this; }
		template<class U> constexpr type& operator>>= (const vec<4, U>& v) { x >>= static_cast<T>(v.x); y >>= static_cast<T>(v.y); z >>= static_cast<T>(v.z); w >>= static_cast<T>(v.w); return *this; }
		
		constexpr type operator+ () { return *this; }
		constexpr type operator- () { return type(-x, -y, -z, -w); }
		
		constexpr type operator+ (const type& v) { return type(x + v.x, y + v.y, z + v.z, w + v.w); }
		constexpr type operator- (const type& v) { return type(x - v.x, y - v.y, z - v.z, w - v.w); }
		constexpr type operator* (const type& v) { return type(x * v.x, y * v.y, z * v.z, w * v.w); }
		constexpr type operator/ (const type& v) { return type(x / v.x, y / v.y, z / v.z, w / v.w); }
		constexpr type operator% (const type& v) { return type(x % v.x, y % v.y, z % v.z, w % v.w); }
		constexpr type operator& (const type& v) { return type(x & v.x, y & v.y, z & v.z, w & v.w); }
		constexpr type operator| (const type& v) { return type(x | v.x, y | v.y, z | v.z, w | v.w); }
		constexpr type operator^ (const type& v) { return type(x ^ v.x, y ^ v.y, z ^ v.z, w ^ v.w); }
		constexpr type operator<< (const type& v) { return type(x << v.x, y << v.y, z << v.z, w << v.w); }
		constexpr type operator>> (const type& v) { return type(x >> v.x, y >> v.y, z >> v.z, w >> v.w); }
		friend constexpr type operator+ (const type& v, T scalar) { return type(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar); }
		friend constexpr type operator- (const type& v, T scalar) { return type(v.x - scalar, v.y - scalar, v.z - scalar, v.w - scalar); }
		friend constexpr type operator* (const type& v, T scalar) { return type(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar); }
		friend constexpr type operator/ (const type& v, T scalar) { return type(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar); }
		friend constexpr type operator% (const type& v, T scalar) { return type(v.x % scalar, v.y % scalar, v.z % scalar, v.w % scalar); }
		friend constexpr type operator& (const type& v, T scalar) { return type(v.x & scalar, v.y & scalar, v.z & scalar, v.w & scalar); }
		friend constexpr type operator| (const type& v, T scalar) { return type(v.x | scalar, v.y | scalar, v.z | scalar, v.w | scalar); }
		friend constexpr type operator^ (const type& v, T scalar) { return type(v.x ^ scalar, v.y ^ scalar, v.z ^ scalar, v.w ^ scalar); }
		friend constexpr type operator<< (const type& v, T scalar) { return type(v.x << scalar, v.y << scalar, v.z << scalar, v.w << scalar); }
		friend constexpr type operator>> (const type& v, T scalar) { return type(v.x >> scalar, v.y >> scalar, v.z >> scalar, v.w >> scalar); }
		friend constexpr type operator+ (T scalar, const type& v) { return type(scalar + v.x, scalar + v.y, scalar + v.z, scalar + v.w); }
		friend constexpr type operator- (T scalar, const type& v) { return type(scalar - v.x, scalar - v.y, scalar - v.z, scalar - v.w); }
		friend constexpr type operator* (T scalar, const type& v) { return type(scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.w); }
		friend constexpr type operator/ (T scalar, const type& v) { return type(scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w); }
		friend constexpr type operator% (T scalar, const type& v) { return type(scalar % v.x, scalar % v.y, scalar % v.z, scalar % v.w); }
		friend constexpr type operator& (T scalar, const type& v) { return type(scalar & v.x, scalar & v.y, scalar & v.z, scalar & v.w); }
		friend constexpr type operator| (T scalar, const type& v) { return type(scalar | v.x, scalar | v.y, scalar | v.z, scalar | v.w); }
		friend constexpr type operator^ (T scalar, const type& v) { return type(scalar ^ v.x, scalar ^ v.y, scalar ^ v.z, scalar ^ v.w); }
		friend constexpr type operator<< (T scalar, const type& v) { return type(scalar << v.x, scalar << v.y, scalar << v.z, scalar << v.w); }
		friend constexpr type operator>> (T scalar, const type& v) { return type(scalar >> v.x, scalar >> v.y, scalar >> v.z, scalar >> v.w); }
		
		friend constexpr type operator+ (const type& v1, const vec<1, T> v2) { return type(v1.x + v2.x, v1.y + v2.x, v1.z + v2.x, v1.w + v2.x); }
		friend constexpr type operator- (const type& v1, const vec<1, T> v2) { return type(v1.x - v2.x, v1.y - v2.x, v1.z - v2.x, v1.w - v2.x); }
		friend constexpr type operator* (const type& v1, const vec<1, T> v2) { return type(v1.x * v2.x, v1.y * v2.x, v1.z * v2.x, v1.w * v2.x); }
		friend constexpr type operator/ (const type& v1, const vec<1, T> v2) { return type(v1.x / v2.x, v1.y / v2.x, v1.z / v2.x, v1.w / v2.x); }
		friend constexpr type operator% (const type& v1, const vec<1, T> v2) { return type(v1.x % v2.x, v1.y % v2.x, v1.z % v2.x, v1.w % v2.x); }
		friend constexpr type operator& (const type& v1, const vec<1, T> v2) { return type(v1.x & v2.x, v1.y & v2.x, v1.z & v2.x, v1.w & v2.x); }
		friend constexpr type operator| (const type& v1, const vec<1, T> v2) { return type(v1.x | v2.x, v1.y | v2.x, v1.z | v2.x, v1.w | v2.x); }
		friend constexpr type operator^ (const type& v1, const vec<1, T> v2) { return type(v1.x ^ v2.x, v1.y ^ v2.x, v1.z ^ v2.x, v1.w ^ v2.x); }
		friend constexpr type operator<< (const type& v1, const vec<1, T> v2) { return type(v1.x << v2.x, v1.y << v2.x, v1.z << v2.x, v1.w << v2.x); }
		friend constexpr type operator>> (const type& v1, const vec<1, T> v2) { return type(v1.x >> v2.x, v1.y >> v2.x, v1.z >> v2.x, v1.w >> v2.x); }
		friend constexpr type operator+ (const vec<1, T> v1, const type& v2) { return type(v1.x + v2.x, v1.x + v2.y, v1.x + v2.z, v1.x + v2.w); }
		friend constexpr type operator- (const vec<1, T> v1, const type& v2) { return type(v1.x - v2.x, v1.x - v2.y, v1.x - v2.z, v1.x - v2.w); }
		friend constexpr type operator* (const vec<1, T> v1, const type& v2) { return type(v1.x * v2.x, v1.x * v2.y, v1.x * v2.z, v1.x * v2.w); }
		friend constexpr type operator/ (const vec<1, T> v1, const type& v2) { return type(v1.x / v2.x, v1.x / v2.y, v1.x / v2.z, v1.x / v2.w); }
		friend constexpr type operator% (const vec<1, T> v1, const type& v2) { return type(v1.x % v2.x, v1.x % v2.y, v1.x % v2.z, v1.x % v2.w); }
		friend constexpr type operator& (const vec<1, T> v1, const type& v2) { return type(v1.x & v2.x, v1.x & v2.y, v1.x & v2.z, v1.x & v2.w); }
		friend constexpr type operator| (const vec<1, T> v1, const type& v2) { return type(v1.x | v2.x, v1.x | v2.y, v1.x | v2.z, v1.x | v2.w); }
		friend constexpr type operator^ (const vec<1, T> v1, const type& v2) { return type(v1.x ^ v2.x, v1.x ^ v2.y, v1.x ^ v2.z, v1.x ^ v2.w); }
		friend constexpr type operator<< (const vec<1, T> v1, const type& v2) { return type(v1.x << v2.x, v1.x << v2.y, v1.x << v2.z, v1.x << v2.w); }
		friend constexpr type operator>> (const vec<1, T> v1, const type& v2) { return type(v1.x >> v2.x, v1.x >> v2.y, v1.x >> v2.z, v1.x >> v2.w); }

		constexpr type operator~ () { return type(~x, ~y, ~z, ~w); }
		
		constexpr bool operator== (const type& v) { return x == v.x && y == v.y && z == v.z && w == v.w; }
		
		//friend constexpr bool_type operator&& (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x && v2.x, v1.y && v2.y); }
		//friend constexpr bool_type operator|| (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x || v2.x, v1.y || v2.y); }

		union
		{
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T s, t, p, q; };
		};
	};
}