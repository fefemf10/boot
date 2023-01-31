export module glm.mat3x2;
export import glm.mat;
export namespace glm
{
	template<typename T>
	struct mat<3, 2, T>
	{
		using col_type = vec<2, T>;
		using row_type = vec<3, T>;
		using type = mat<3, 2, T>;
		using transpose_type = mat<2, 3, T>;
		using value_type = T;

		constexpr static size_t length() { return 3; }
		col_type& operator[](size_t i) { return value[i]; }
		constexpr const col_type& operator[](size_t i) const { return value[i]; }

		constexpr mat() = default;
		constexpr mat(const type& m) = default;

		constexpr explicit mat(T scalar) : value{ col_type(scalar, 0), col_type(0, scalar), col_type(0) } {}
		constexpr mat(T x1, T y1, T x2, T y2, T x3, T y3) : value{ col_type(x1, y1), col_type(x2, y2), col_type(x3, y3) } {}
		constexpr mat(const col_type& v1, const col_type& v2, const col_type& v3) : value{ v1, v2, v3 } {}

		template <class X1, class Y1, class X2, class Y2, class X3, class Y3> constexpr mat(X1 x1, Y1 y1, X2 x2, Y2 y2, X3 x3, Y3 y3) : value{ col_type(x1, y1), col_type(x2, y2), col_type(x3, y3) } {}
		template <class V1, class V2, class V3> constexpr mat(const vec<2, V1>& v1, const vec<2, V2>& v2, const vec<2, V3>& v3) : value{ col_type(v1), col_type(v2), col_type(v3) } {}


		template <class U> constexpr explicit mat(const mat<2, 2, U>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<2, 3, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<2, 4, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<3, 2, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<3, 3, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<3, 4, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<4, 2, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<4, 3, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}
		template <class U> constexpr explicit mat(const mat<4, 4, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]) } {}

		template <class U> constexpr type& operator= (U scalar) { value[0] = scalar; value[1] = scalar; value[2] = scalar; return *this; }
		template <class U> constexpr type& operator+= (U scalar) { value[0] += scalar; value[1] += scalar; value[2] += scalar; return *this; }
		template <class U> constexpr type& operator-= (U scalar) { value[0] -= scalar; value[1] -= scalar; value[2] -= scalar; return *this; }
		template <class U> constexpr type& operator*= (U scalar) { value[0] *= scalar; value[1] *= scalar; value[2] *= scalar; return *this; }
		template <class U> constexpr type& operator/= (U scalar) { value[0] /= scalar; value[1] /= scalar; value[2] /= scalar; return *this; }
		template <class U> constexpr type& operator= (const mat<3, 2, U>& m) { value[0] = m[0]; value[1] = m[1]; value[2] = m[2]; return *this; }
		template <class U> constexpr type& operator+= (const mat<3, 2, U>& m) { value[0] += m[0]; value[1] += m[1]; value[2] += m[2]; return *this; }
		template <class U> constexpr type& operator-= (const mat<3, 2, U>& m) { value[0] -= m[0]; value[1] -= m[1]; value[2] -= m[2]; return *this; }

		constexpr type& operator++() { ++value[0]; ++value[1]; ++value[2]; return *this; }
		constexpr type& operator--() { --value[0]; --value[1]; --value[2]; return *this; }
		constexpr type operator++(int) { type result(*this); ++* this; return result; }
		constexpr type operator--(int) { type result(*this); --* this; return result; }

		constexpr type operator+ () { return *this; }
		constexpr type operator- () { return type(-value[0], -value[1], -value[2]); }

		friend constexpr type operator+ (const type& m, T scalar) { return type(m[0] + scalar, m[1] + scalar, m[2] + scalar); }
		friend constexpr type operator- (const type& m, T scalar) { return type(m[0] - scalar, m[1] - scalar, m[2] - scalar); }
		friend constexpr type operator* (const type& m, T scalar) { return type(m[0] * scalar, m[1] * scalar, m[2] * scalar); }
		friend constexpr type operator/ (const type& m, T scalar) { return type(m[0] / scalar, m[1] / scalar, m[2] / scalar); }
		friend constexpr type operator+ (T scalar, const type& m) { return type(scalar + m[0], scalar + m[1], scalar + m[2]); }
		friend constexpr type operator- (T scalar, const type& m) { return type(scalar - m[0], scalar - m[1], scalar - m[2]); }
		friend constexpr type operator* (T scalar, const type& m) { return type(scalar * m[0], scalar * m[1], scalar * m[2]); }
		friend constexpr type operator/ (T scalar, const type& m) { return type(scalar / m[0], scalar / m[1], scalar / m[2]); }
		friend constexpr type operator+ (const type& m1, const type& m2) { return type(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]); }
		friend constexpr type operator- (const type& m1, const type& m2) { return type(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]); }
		friend constexpr typename type::col_type operator* (const type& m, const typename type::row_type& v) { return col_type(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z, m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z); }
		friend constexpr typename type::row_type operator* (const typename type::col_type& v, const type& m) { return row_type(v.x * m[0][0] + v.y * m[0][1], v.x * m[1][0] + v.y * m[1][1], v.x * m[2][0] + v.y * m[2][1]); }
		friend constexpr mat<2, 2, T> operator* (const type& m1, const mat<2, 3, T>& m2)
		{
			T SrcA00 = m1[0][0];
			T SrcA01 = m1[0][1];
			T SrcA10 = m1[1][0];
			T SrcA11 = m1[1][1];
			T SrcA20 = m1[2][0];
			T SrcA21 = m1[2][1];

			T SrcB00 = m2[0][0];
			T SrcB01 = m2[0][1];
			T SrcB02 = m2[0][2];
			T SrcB10 = m2[1][0];
			T SrcB11 = m2[1][1];
			T SrcB12 = m2[1][2];

			mat<2, 2, T> result;
			result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
			result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
			result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
			result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
			return result;
		}
		friend constexpr mat<3, 2, T> operator* (const type& m1, const mat<3, 3, T>& m2)
		{
			return mat<3, 2, T>(
				m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
				m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
				m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
				m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
				m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
				m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2]);
		}
		friend constexpr mat<4, 2, T> operator* (const type& m1, const mat<4, 3, T>& m2)
		{
			return mat<4, 2, T>(
				m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
				m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
				m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
				m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
				m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
				m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
				m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2],
				m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2]);
		}

		constexpr typename type::transpose_type transpose()
		{
			typename type::transpose_type result(value[0][0], value[1][0], value[2][0], value[1][0], value[1][1], value[1][2]);
			return result;
		}

	private:
		col_type value[3];
	};
}