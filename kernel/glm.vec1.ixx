export module glm.vec1;
export import glm.vec;

export namespace glm
{
	template<typename T>
	struct vec<1, T>
	{
		using value_type = T;
		using type = vec<1, T>;
		using bool_type = vec<1, bool>;

		constexpr static size_t length() { return 1; }
		T& operator[](size_t i) { return x; }
		constexpr const T& operator[](size_t i) const { return x; }

		constexpr vec() = default;
		constexpr vec(const type& v) = default;
		
		constexpr explicit vec(T scalar) : x(scalar) {}
		
		template<class A> constexpr vec(A x) : x(static_cast<T>(x)) {}
		
		template<class U> constexpr explicit vec(const vec<1, U>& v) : x(static_cast<T>(v.x)) {}
		template<class U> constexpr explicit vec(const vec<2, U>& v) : x(static_cast<T>(v.x)) {}
		template<class U> constexpr explicit vec(const vec<3, U>& v) : x(static_cast<T>(v.x)) {}
		template<class U> constexpr explicit vec(const vec<4, U>& v) : x(static_cast<T>(v.x)) {}
		
		constexpr type& operator= (const type& v) { x = v.x; return *this; };
		template<class U> constexpr type& operator= (const vec<1, U>& v) { x = static_cast<T>(v.x); return *this; }
		
		template<class U> constexpr type& operator+= (U scalar) { x += static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator-= (U scalar) { x -= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator*= (U scalar) { x *= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator/= (U scalar) { x /= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator+= (const vec<1, U>& v) { x += static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator-= (const vec<1, U>& v) { x -= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator*= (const vec<1, U>& v) { x *= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator/= (const vec<1, U>& v) { x /= static_cast<T>(v.x); return *this; }
		
		constexpr type& operator++() { ++x; return *this; }
		constexpr type& operator--() { --x; return *this; }
		constexpr type operator++(int) { type result(*this); ++* this; return result; }
		constexpr type operator--(int) { type result(*this); --* this; return result; }
		
		template<class U> constexpr type& operator%= (U scalar) { x %= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator&= (U scalar) { x &= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator|= (U scalar) { x |= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator^= (U scalar) { x ^= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator<<= (U scalar) { x <<= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator>>= (U scalar) { x >>= static_cast<T>(scalar); return *this; }
		template<class U> constexpr type& operator%= (const vec<1, U>& v) { x %= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator&= (const vec<1, U>& v) { x &= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator|= (const vec<1, U>& v) { x |= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator^= (const vec<1, U>& v) { x ^= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator<<= (const vec<1, U>& v) { x <<= static_cast<T>(v.x); return *this; }
		template<class U> constexpr type& operator>>= (const vec<1, U>& v) { x >>= static_cast<T>(v.x); return *this; }
		
		constexpr type operator+ () { return *this; }
		constexpr type operator- () { return type(-x); }
		
		constexpr type operator+ (const type& v) const { return type(x + v.x); }
		constexpr type operator- (const type& v) const { return type(x - v.x); }
		constexpr type operator* (const type& v) const { return type(x * v.x); }
		constexpr type operator/ (const type& v) const { return type(x / v.x); }
		constexpr type operator% (const type& v) const { return type(x % v.x); }
		constexpr type operator& (const type& v) const { return type(x & v.x); }
		constexpr type operator| (const type& v) const { return type(x | v.x); }
		constexpr type operator^ (const type& v) const { return type(x ^ v.x); }
		constexpr type operator<< (const type& v) const { return type(x << v.x); }
		constexpr type operator>> (const type& v) const { return type(x >> v.x); }
		friend constexpr type operator+ (const type& v, T scalar) { return type(v.x + scalar); }
		friend constexpr type operator- (const type& v, T scalar) { return type(v.x - scalar); }
		friend constexpr type operator* (const type& v, T scalar) { return type(v.x * scalar); }
		friend constexpr type operator/ (const type& v, T scalar) { return type(v.x / scalar); }
		friend constexpr type operator% (const type& v, T scalar) { return type(v.x % scalar); }
		friend constexpr type operator& (const type& v, T scalar) { return type(v.x & scalar); }
		friend constexpr type operator| (const type& v, T scalar) { return type(v.x | scalar); }
		friend constexpr type operator^ (const type& v, T scalar) { return type(v.x ^ scalar); }
		friend constexpr type operator<< (const type& v, T scalar) { return type(v.x << scalar); }
		friend constexpr type operator>> (T scalar, const type& v) { return type(scalar >> v.x); }
		friend constexpr type operator+ (T scalar, const type& v) { return type(scalar + v.x); }
		friend constexpr type operator- (T scalar, const type& v) { return type(scalar - v.x); }
		friend constexpr type operator* (T scalar, const type& v) { return type(scalar * v.x); }
		friend constexpr type operator/ (T scalar, const type& v) { return type(scalar / v.x); }
		friend constexpr type operator% (T scalar, const type& v) { return type(scalar % v.x); }
		friend constexpr type operator& (T scalar, const type& v) { return type(scalar & v.x); }
		friend constexpr type operator| (T scalar, const type& v) { return type(scalar | v.x); }
		friend constexpr type operator^ (T scalar, const type& v) { return type(scalar ^ v.x); }
		friend constexpr type operator<< (T scalar, const type& v) { return type(scalar << v.x, scalar << v.y); }
		friend constexpr type operator>> (const type& v, T scalar) { return type(v.x >> scalar, v.y >> scalar); }
		
		constexpr type operator~ () { return type(~x); }
		
		constexpr bool operator== (const type& v) { return x == v.x; }
		
		//friend constexpr bool_type operator&& (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x && v2.x, v1.y && v2.y); }
		//friend constexpr bool_type operator|| (const bool_type& v1, const bool_type& v2) { return bool_type(v1.x || v2.x, v1.y || v2.y); }
		
		union
		{
			struct { T x; };
			struct { T r; };
			struct { T s; };
		};
	};
}