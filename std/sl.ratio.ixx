export module sl.ratio;
import sl.numeric_limits;
import sl.typetraits;
namespace std
{
	[[nodiscard]] constexpr long long abs(const long long value) noexcept {
		return value < 0 ? -value : value;
	}
	template <long long A, long long B, bool _Sfinae = false,
		bool _Good = abs(A) <= numeric_limits<long long>::max() / (B == 0 ? 1 : abs(B))>
	struct _Safe_mult : integral_constant<long long, A* B> {};

	template <long long A, long long B, bool _Sfinae>
	struct _Safe_mult<A, B, _Sfinae, false>
	{
		static_assert(_Sfinae, "integer arithmetic overflow");
	};

	[[nodiscard]] constexpr long long sign(const long long value) noexcept
	{
		return value < 0 ? -1 : 1;
	}

	inline void _Safe_add_integer_arithmetic_overflow_error() noexcept {}

	[[nodiscard]] constexpr long long _Safe_add(const long long A, const long long B) noexcept {
		if (sign(A) == sign(B) && abs(A) > numeric_limits<long long>::max() - abs(B))
			_Safe_add_integer_arithmetic_overflow_error();
		return A + B;
	}

	[[nodiscard]] constexpr long long gcd(long long A, long long B) noexcept {
		if (A == 0 && B == 0)
			return 1;
		A = abs(A);
		B = abs(B);
		while (B != 0)
		{
			const long long A2 = A;
			A = B;
			B = A2 % B;
		}
		return A;
	}
	template <long long N, long long D>
	struct ratio;

	template <class T>
	inline constexpr bool is_ravio_v = false;

	template <long long T1, long long T2>
	inline constexpr bool is_ravio_v<ratio<T1, T2>> = true;

	template <class T1, class T2>
	struct _Ratio_add
	{
		static_assert(is_ravio_v<T1>&& is_ravio_v<T2>, "ratio_add<T1, T2> requires T1 and T2 to be ratio<>s.");

		static constexpr long long N1 = T1::num;
		static constexpr long long D1 = T1::den;
		static constexpr long long N2 = T2::num;
		static constexpr long long D2 = T2::den;

		static constexpr long long G = gcd(D1, D2);

		using type = typename ratio<_Safe_add(_Safe_mult<N1, D2 / G>::value, _Safe_mult<N2, D1 / G>::value), _Safe_mult<D1, D2 / G>::value>::type;
	};

	template <class T1, class T2>
	struct _Ratio_subtract
	{
		static_assert(is_ravio_v<T1>&& is_ravio_v<T2>, "ratio_subtract<R1, R2> requires R1 and R2 to be ratio<>s.");

		static constexpr long long N2 = T2::num;
		static constexpr long long D2 = T2::den;

		using type = _Ratio_add<T1, ratio<-N2, D2>>;
	};

	template <class T1, class T2>
	struct _Ratio_multiply
	{
		static_assert(is_ravio_v<T1>&& is_ravio_v<T2>, "ratio_multiply<R1, R2> requires R1 and R2 to be ratio<>s.");

		static constexpr long long N1 = T1::num;
		static constexpr long long D1 = T1::den;
		static constexpr long long N2 = T2::num;
		static constexpr long long D2 = T2::den;

		static constexpr long long GX = gcd(N1, D2);
		static constexpr long long GY = gcd(N2, D1);

		using _Num = _Safe_mult<N1 / GX, N2 / GY, true>;
		using _Den = _Safe_mult<D1 / GY, D2 / GX, true>;
	};

	template <class T1, class T2, bool _Sfinae = true, class = void>
	struct _Ratio_multiply_sfinae
	{
		static_assert(_Sfinae, "integer arithmetic overflow");
	};

	template <class T1, class T2, bool _Sfinae>
	struct _Ratio_multiply_sfinae<T1, T2, _Sfinae, void_t<typename _Ratio_multiply<T1, T2>::_Num::type, typename _Ratio_multiply<T1, T2>::_Den::type>>
	{
		using type = ratio<_Ratio_multiply<T1, T2>::_Num::value, _Ratio_multiply<T1, T2>::_Den::value>;
	};

	template <class T1, class T2>
	struct _Ratio_divide
	{
		static_assert(is_ravio_v<T1>&& is_ravio_v<T2>, "ratio_divide<R1, R2> requires R1 and R2 to be ratio<>s.");

		static constexpr long long N2 = T2::num;
		static constexpr long long D2 = T2::den;

		using T2_inverse = ratio<D2, N2>;
	};

	template <class T1, class T2, bool _Sfinae = true>
	using _Ratio_divide_sfinae = typename _Ratio_multiply_sfinae<T1, typename _Ratio_divide<T1, T2>::T2_inverse, _Sfinae>::type;

	struct _Big_uint128 {
		unsigned long long upper;
		unsigned long long lower;

		constexpr bool operator<(const _Big_uint128 rhs) const noexcept {
			if (upper != rhs.upper) {
				return upper < rhs.upper;
			}

			return lower < rhs.lower;
		}
	};

	constexpr _Big_uint128 _Big_multiply(const unsigned long long lfactor, const unsigned long long rfactor) noexcept
	{
		const unsigned long long llow = lfactor & 0xFFFF'FFFFULL;
		const unsigned long long lhigh = lfactor >> 32;
		const unsigned long long rlow = rfactor & 0xFFFF'FFFFULL;
		const unsigned long long rhigh = rfactor >> 32;

		unsigned long long temp = llow * rlow;
		const unsigned long long lower32 = temp & 0xFFFF'FFFFULL;
		unsigned long long carry = temp >> 32;

		temp = llow * rhigh + carry;
		const unsigned long long mid_lower = temp & 0xFFFF'FFFFULL;
		const unsigned long long mid_upper = temp >> 32;

		temp = lhigh * rlow + mid_lower;
		carry = temp >> 32;

		return { lhigh * rhigh + mid_upper + carry, (temp << 32) + lower32 };
	}

	constexpr bool _Ratio_less(const long long N1, const long long D1, const long long N2, const long long D2) noexcept {
		if (N1 >= 0 && N2 >= 0) {
			return _Big_multiply(static_cast<unsigned long long>(N1), static_cast<unsigned long long>(D2))
				< _Big_multiply(static_cast<unsigned long long>(N2), static_cast<unsigned long long>(D1));
		}

		if (N1 < 0 && N2 < 0) {
			return _Big_multiply(static_cast<unsigned long long>(-N2), static_cast<unsigned long long>(D1))
				< _Big_multiply(static_cast<unsigned long long>(-N1), static_cast<unsigned long long>(D2));
		}

		return N1 < N2;
	}

	
}

export namespace std
{
	template <long long N, long long D = 1>
	struct ratio
	{
		static_assert(D != 0, "zero denominator");
		static_assert(-numeric_limits<long long>::max() <= N, "numerator too negative");
		static_assert(-numeric_limits<long long>::max() <= D, "denominator too negative");
		
		static constexpr long long num = sign(N) * sign(D) * abs(N) / gcd(N, D);
		static constexpr long long den = abs(D) / gcd(N, D);

		using type = ratio<num, den>;
	};

	template <class T1, class T2>
	using ratio_add = typename _Ratio_add<T1, T2>::type;
	template <class T1, class T2>
	using ratio_subtract = typename _Ratio_subtract<T1, T2>::type;
	template <class T1, class T2>
	using ratio_multiply = typename _Ratio_multiply_sfinae<T1, T2, false>::type;
	template <class T1, class T2>
	using ratio_divide = _Ratio_divide_sfinae<T1, T2, false>;

	template <class T1, class T2>
	struct ratio_equal : bool_constant<T1::num == T2::num && T1::den == T2::den>
	{
		static_assert(is_ratio_v<T1>&& is_ratio_v<T2>, "ratio_equal<R1, R2> requires R1 and R2 to be ratio<>s.");
	};

	template <class T1, class T2>
	inline constexpr bool ratio_equal_v = ratio_equal<T1, T2>::value;

	template <class T1, class T2>
	struct ratio_not_equal : bool_constant<!ratio_equal_v<T1, T2>>
	{
		static_assert(is_ratio_v<T1>&& is_ratio_v<T2>, "ratio_not_equal<R1, R2> requires R1 and R2 to be ratio<>s.");
	};

	template <class T1, class T2>
	inline constexpr bool ratio_not_equal_v = ratio_not_equal<T1, T2>::value;

	template <class T1, class T2>
	struct ratio_less : bool_constant<_Ratio_less(T1::num, T1::den, T2::num, T2::den)>
	{
		static_assert(is_ratio_v<T1>&& is_ratio_v<T2>, "ratio_less<R1, R2> requires R1 and R2 to be ratio<>s.");
	};

	template <class T1, class T2>
	inline constexpr bool ratio_less_v = ratio_less<T1, T2>::value;

	template <class T1, class T2>
	struct ratio_less_equal : bool_constant<!ratio_less_v<T2, T1>>
	{
		static_assert(
			is_ratio_v<T1>&& is_ratio_v<T2>, "ratio_less_equal<R1, R2> requires R1 and R2 to be ratio<>s.");
	};

	template <class T1, class T2>
	inline constexpr bool ratio_less_equal_v = ratio_less_equal<T1, T2>::value;

	template <class T1, class T2>
	struct ratio_greater : ratio_less<T2, T1>::type
	{
		static_assert(is_ratio_v<T1>&& is_ratio_v<T2>, "ratio_greater<R1, R2> requires R1 and R2 to be ratio<>s.");
	};

	template <class T1, class T2>
	inline constexpr bool ratio_greater_v = ratio_greater<T1, T2>::value;

	template <class T1, class T2>
	struct ratio_greater_equal : bool_constant<!ratio_less_v<T1, T2>>
	{
		static_assert(is_ratio_v<T1>&& is_ratio_v<T2>, "ratio_greater_equal<R1, R2> requires R1 and R2 to be ratio<>s.");
	};

	template <class T1, class T2>
	inline constexpr bool ratio_greater_equal_v = ratio_greater_equal<T1, T2>::value;

	using atto = ratio<1, 1000000000000000000LL>;
	using femto = ratio<1, 1000000000000000LL>;
	using pico = ratio<1, 1000000000000LL>;
	using nano = ratio<1, 1000000000>;
	using micro = ratio<1, 1000000>;
	using milli = ratio<1, 1000>;
	using centi = ratio<1, 100>;
	using deci = ratio<1, 10>;
	using deca = ratio<10, 1>;
	using hecto = ratio<100, 1>;
	using kilo = ratio<1000, 1>;
	using mega = ratio<1000000, 1>;
	using giga = ratio<1000000000, 1>;
	using tera = ratio<1000000000000LL, 1>;
	using peta = ratio<1000000000000000LL, 1>;
	using exa = ratio<1000000000000000000LL, 1>;
}