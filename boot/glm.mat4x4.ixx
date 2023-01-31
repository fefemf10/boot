export module glm.mat4x4;
export import glm.mat;
export namespace glm
{
	template<typename T>
	struct mat<4, 4, T>
	{
		using col_type = vec<4, T>;
		using row_type = vec<4, T>;
		using type = mat<4, 4, T>;
		using transpose_type = mat<4, 4, T>;
		using value_type = T;

		constexpr static size_t length() { return 4; }
		col_type& operator[](size_t i) { return value[i]; }
		constexpr const col_type& operator[](size_t i) const { return value[i]; }

		constexpr mat() = default;
		constexpr mat(const type& m) = default;

		constexpr explicit mat(T scalar) : value{ col_type(scalar, 0, 0, 0), col_type(0, scalar, 0, 0), col_type(0, 0, scalar, 0), col_type(0, 0, 0, scalar) } {}
		constexpr mat(T x1, T y1, T z1, T w1, T x2, T y2, T z2, T w2, T x3, T y3, T z3, T w3, T x4, T y4, T z4, T w4) : value{ col_type(x1, y1, z1, w1), col_type(x2, y2, z2, w2), col_type(x3, y3, z3, w3), col_type(x4, y4, z4, w4) } {}
		constexpr mat(const col_type& v1, const col_type& v2, const col_type& v3, const col_type& v4) : value{ v1, v2, v3, v4 } {}

		template <class X1, class Y1, class Z1, class W1, class X2, class Y2, class Z2, class W2, class X3, class Y3, class Z3, class W3, class X4, class Y4, class Z4, class W4> constexpr mat(X1 x1, Y1 y1, Z1 z1, W1 w1, X2 x2, Y2 y2, Z2 z2, W2 w2, X3 x3, Y3 y3, Z3 z3, W3 w3, X4 x4, Y4 y4, Z4 z4, W4 w4) : value{ col_type(x1, y1, z1, w1), col_type(x2, y2, z2, w2), col_type(x3, y3, z3, w3), col_type(x4, y4, z4, w4) } {}
		template <class V1, class V2, class V3, class V4> constexpr mat(const vec<4, V1>& v1, const vec<4, V2>& v2, const vec<4, V3>& v3, const vec<4, V4>& v4) : value{ col_type(v1), col_type(v2), col_type(v3), col_type(v4) } {}


		template <class U> constexpr explicit mat(const mat<2, 2, U>& m) : value{ col_type(m[0], 0, 0), col_type(m[1], 0, 0), col_type(0), col_type(0) } {}
		template <class U> constexpr explicit mat(const mat<2, 3, T>& m) : value{ col_type(m[0], 0), col_type(m[1], 0), col_type(0), col_type(0) } {}
		template <class U> constexpr explicit mat(const mat<2, 4, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(0), col_type(0) } {}
		template <class U> constexpr explicit mat(const mat<3, 2, T>& m) : value{ col_type(m[0], 0, 0), col_type(m[1], 0, 0), col_type(m[2], 0, 0), col_type(0) } {}
		template <class U> constexpr explicit mat(const mat<3, 3, T>& m) : value{ col_type(m[0], 0), col_type(m[1], 0), col_type(m[2], 0), col_type(0) } {}
		template <class U> constexpr explicit mat(const mat<3, 4, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(0) } {}
		template <class U> constexpr explicit mat(const mat<4, 2, T>& m) : value{ col_type(m[0], 0, 0), col_type(m[1], 0, 0), col_type(m[2], 0, 0), col_type(m[3], 0, 0) } {}
		template <class U> constexpr explicit mat(const mat<4, 3, T>& m) : value{ col_type(m[0], 0), col_type(m[1], 0), col_type(m[2], 0), col_type(m[3], 0) } {}
		template <class U> constexpr explicit mat(const mat<4, 4, T>& m) : value{ col_type(m[0]), col_type(m[1]), col_type(m[2]), col_type(m[3]) } {}

		template <class U> constexpr type& operator= (U scalar) { value[0] = scalar; value[1] = scalar; value[2] = scalar; value[3] = scalar; return *this; }
		template <class U> constexpr type& operator+= (U scalar) { value[0] += scalar; value[1] += scalar; value[2] += scalar; value[3] += scalar; return *this; }
		template <class U> constexpr type& operator-= (U scalar) { value[0] -= scalar; value[1] -= scalar; value[2] -= scalar; value[3] -= scalar; return *this; }
		template <class U> constexpr type& operator*= (U scalar) { value[0] *= scalar; value[1] *= scalar; value[2] *= scalar; value[3] *= scalar; return *this; }
		template <class U> constexpr type& operator/= (U scalar) { value[0] /= scalar; value[1] /= scalar; value[2] /= scalar; value[3] /= scalar; return *this; }
		template <class U> constexpr type& operator= (const mat<4, 4, U>& m) { value[0] = m[0]; value[1] = m[1]; value[2] = m[2]; value[3] = m[3]; return *this; }
		template <class U> constexpr type& operator+= (const mat<4, 4, U>& m) { value[0] += m[0]; value[1] += m[1]; value[2] += m[2]; value[3] += m[3]; return *this; }
		template <class U> constexpr type& operator-= (const mat<4, 4, U>& m) { value[0] -= m[0]; value[1] -= m[1]; value[2] -= m[2]; value[3] -= m[3]; return *this; }
		template <class U> constexpr type& operator*= (const mat<4, 4, U>& m) { return *this = *this * m; }
		template <class U> constexpr type& operator/= (const mat<4, 4, U>& m) { return *this *= inverse(m); }

		constexpr type& operator++() { ++value[0]; ++value[1]; ++value[2]; ++value[3]; return *this; }
		constexpr type& operator--() { --value[0]; --value[1]; --value[2]; --value[3]; return *this; }
		constexpr type operator++(int) { type result(*this); ++* this; return result; }
		constexpr type operator--(int) { type result(*this); --* this; return result; }

		constexpr type operator+ () { return *this; }
		constexpr type operator- () { return type(-value[0], -value[1], -value[2], -value[3]); }

		friend constexpr type operator+ (const type& m, T scalar) { return type(m[0] + scalar, m[1] + scalar, m[2] + scalar, m[3] + scalar); }
		friend constexpr type operator- (const type& m, T scalar) { return type(m[0] - scalar, m[1] - scalar, m[2] - scalar, m[3] - scalar); }
		friend constexpr type operator* (const type& m, T scalar) { return type(m[0] * scalar, m[1] * scalar, m[2] * scalar, m[3] * scalar); }
		friend constexpr type operator/ (const type& m, T scalar) { return type(m[0] / scalar, m[1] / scalar, m[2] / scalar, m[3] / scalar); }
		friend constexpr type operator+ (T scalar, const type& m) { return type(scalar + m[0], scalar + m[1], scalar + m[2], scalar + m[3]); }
		friend constexpr type operator- (T scalar, const type& m) { return type(scalar - m[0], scalar - m[1], scalar - m[2], scalar - m[3]); }
		friend constexpr type operator* (T scalar, const type& m) { return type(scalar * m[0], scalar * m[1], scalar * m[2], scalar * m[3]); }
		friend constexpr type operator/ (T scalar, const type& m) { return type(scalar / m[0], scalar / m[1], scalar / m[2], scalar / m[3]); }
		friend constexpr type operator+ (const type& m1, const type& m2) { return type(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]); }
		friend constexpr type operator- (const type& m1, const type& m2) { return type(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3]); }
		friend constexpr typename type::col_type operator* (const type& m, const typename type::row_type& v) { return col_type(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w, m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w, m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w, m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w); }
		friend constexpr typename type::row_type operator* (const typename type::col_type& v, const type& m) { return row_type(v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3], v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3], v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3], v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + v.w * m[3][3]); }
		friend constexpr mat<2, 4, T> operator* (const type& m1, const mat<2, 4, T>& m2)
		{
			return mat<2, 4, T>(
				m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
				m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
				m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
				m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3],
				m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
				m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
				m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
				m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3]);
		}
		friend constexpr mat<3, 4, T> operator* (const type& m1, const mat<3, 4, T>& m2)
		{
			return mat<3, 4, T>(
				m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
				m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
				m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
				m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3],
				m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
				m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
				m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
				m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3],
				m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
				m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3],
				m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3],
				m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3]);
		}
		friend constexpr mat<4, 4, T> operator* (const type& m1, const mat<4, 4, T>& m2)
		{
			col_type SrcA0 = m1[0];
			col_type SrcA1 = m1[1];
			col_type SrcA2 = m1[2];
			col_type SrcA3 = m1[3];

			col_type SrcB0 = m2[0];
			col_type SrcB1 = m2[1];
			col_type SrcB2 = m2[2];
			col_type SrcB3 = m2[3];

			mat<4, 4, T> result;
			result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
			result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
			result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
			result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
			return result;
		}
		friend constexpr typename type::col_type operator/ (const type& m, const typename type::row_type& v) { return inverse(m) * v; }
		friend constexpr typename type::row_type operator/ (const typename type::col_type& v, const type& m) { return v * inverse(m); }
		friend constexpr type operator/ (const type& m1, const type& m2) { type m1_copy(m1); return m1_copy /= m2; }

		constexpr typename type::transpose_type transpose()
		{
			typename type::transpose_type result(value[0][0], value[1][0], value[2][0], value[3][0], value[0][1], value[1][1], value[2][1], value[3][1], value[0][2], value[1][2], value[2][2], value[3][2], value[0][3], value[1][3], value[2][3], value[3][3]);
			return result;
		}
		constexpr T deterinant()
		{
			T SubFactor00 = value[2][2] * value[3][3] - value[3][2] * value[2][3];
			T SubFactor01 = value[2][1] * value[3][3] - value[3][1] * value[2][3];
			T SubFactor02 = value[2][1] * value[3][2] - value[3][1] * value[2][2];
			T SubFactor03 = value[2][0] * value[3][3] - value[3][0] * value[2][3];
			T SubFactor04 = value[2][0] * value[3][2] - value[3][0] * value[2][2];
			T SubFactor05 = value[2][0] * value[3][1] - value[3][0] * value[2][1];

			vec<4, T> DetCof(
				(value[1][1] * SubFactor00 - value[1][2] * SubFactor01 + value[1][3] * SubFactor02),
				-(value[1][0] * SubFactor00 - value[1][2] * SubFactor03 + value[1][3] * SubFactor04),
				(value[1][0] * SubFactor01 - value[1][1] * SubFactor03 + value[1][3] * SubFactor05),
				-(value[1][0] * SubFactor02 - value[1][1] * SubFactor04 + value[1][2] * SubFactor05));

			return value[0][0] * DetCof[0] + value[0][1] * DetCof[1] + value[0][2] * DetCof[2] + value[0][3] * DetCof[3];
		}
		constexpr type inverse()
		{
			T Coef00 = value[2][2] * value[3][3] - value[3][2] * value[2][3];
			T Coef02 = value[1][2] * value[3][3] - value[3][2] * value[1][3];
			T Coef03 = value[1][2] * value[2][3] - value[2][2] * value[1][3];

			T Coef04 = value[2][1] * value[3][3] - value[3][1] * value[2][3];
			T Coef06 = value[1][1] * value[3][3] - value[3][1] * value[1][3];
			T Coef07 = value[1][1] * value[2][3] - value[2][1] * value[1][3];

			T Coef08 = value[2][1] * value[3][2] - value[3][1] * value[2][2];
			T Coef10 = value[1][1] * value[3][2] - value[3][1] * value[1][2];
			T Coef11 = value[1][1] * value[2][2] - value[2][1] * value[1][2];

			T Coef12 = value[2][0] * value[3][3] - value[3][0] * value[2][3];
			T Coef14 = value[1][0] * value[3][3] - value[3][0] * value[1][3];
			T Coef15 = value[1][0] * value[2][3] - value[2][0] * value[1][3];

			T Coef16 = value[2][0] * value[3][2] - value[3][0] * value[2][2];
			T Coef18 = value[1][0] * value[3][2] - value[3][0] * value[1][2];
			T Coef19 = value[1][0] * value[2][2] - value[2][0] * value[1][2];

			T Coef20 = value[2][0] * value[3][1] - value[3][0] * value[2][1];
			T Coef22 = value[1][0] * value[3][1] - value[3][0] * value[1][1];
			T Coef23 = value[1][0] * value[2][1] - value[2][0] * value[1][1];

			vec<4, T> Fac0(Coef00, Coef00, Coef02, Coef03);
			vec<4, T> Fac1(Coef04, Coef04, Coef06, Coef07);
			vec<4, T> Fac2(Coef08, Coef08, Coef10, Coef11);
			vec<4, T> Fac3(Coef12, Coef12, Coef14, Coef15);
			vec<4, T> Fac4(Coef16, Coef16, Coef18, Coef19);
			vec<4, T> Fac5(Coef20, Coef20, Coef22, Coef23);

			vec<4, T> Vec0(value[1][0], value[0][0], value[0][0], value[0][0]);
			vec<4, T> Vec1(value[1][1], value[0][1], value[0][1], value[0][1]);
			vec<4, T> Vec2(value[1][2], value[0][2], value[0][2], value[0][2]);
			vec<4, T> Vec3(value[1][3], value[0][3], value[0][3], value[0][3]);

			vec<4, T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
			vec<4, T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
			vec<4, T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
			vec<4, T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

			vec<4, T> SignA(+1, -1, +1, -1);
			vec<4, T> SignB(-1, +1, -1, +1);
			mat<4, 4, T> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

			vec<4, T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

			vec<4, T> Dot0(value[0] * Row0);
			T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

			T OneOverDeterminant = static_cast<T>(1) / Dot1;

			return Inverse * OneOverDeterminant;
		}
	private:
		col_type value[4];
	};
}