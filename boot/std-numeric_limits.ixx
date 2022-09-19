export module std:numeric_limits;

namespace std
{
	constexpr char CHAR_MIN = -128;
	constexpr char CHAR_MAX = 127;
	constexpr signed char SCHAR_MIN = -128;
	constexpr signed char SCHAR_MAX = 127;
	constexpr unsigned char UCHAR_MIN = 0;
	constexpr unsigned char UCHAR_MAX = 255;
	constexpr short SHRT_MIN = -32768;
	constexpr short SHRT_MAX = 32767;
	constexpr unsigned short USHRT_MIN = 0;
	constexpr unsigned short USHRT_MAX = 65535;
	constexpr int INT_MIN = -2147483648;
	constexpr int INT_MAX = 2147483647;
	constexpr unsigned int UINT_MIN = 0;
	constexpr unsigned int UINT_MAX = 4294967295;
	constexpr long long LLONG_MIN = -9223372036854775808i64;
	constexpr long long LLONG_MAX = 9223372036854775807;
	constexpr unsigned long long ULLONG_MIN = 0;
	constexpr unsigned long long ULLONG_MAX = 18446744073709551615;
	constexpr float FLT_MIN = 0;
	constexpr float FLT_MAX = 2147483;
	constexpr double DBL_MIN = 0;
	constexpr double DBL_MAX = 922337203685477;
}

export namespace std
{
	struct numeric_base
	{
		static constexpr bool has_denorm_loss = false;
		static constexpr bool has_infinity = false;
		static constexpr bool has_quiet_NaN = false;
		static constexpr bool has_signaling_NaN = false;
		static constexpr bool is_exact = false;
		static constexpr bool is_iec559 = false;
		static constexpr bool is_integer = false;
		static constexpr bool is_modulo = false;
		static constexpr bool is_signed = false;
		static constexpr bool is_specialized = false;
		static constexpr bool tinyness_before = false;
		static constexpr bool traps = false;
		static constexpr int digits = 0;
		static constexpr int digits10 = 0;
		static constexpr int max_digits10 = 0;
		static constexpr int max_exponent = 0;
		static constexpr int max_exponent10 = 0;
		static constexpr int min_exponent = 0;
		static constexpr int radix = 0;
	};
	template <class T>
	struct numeric_limits : public numeric_base
	{
		[[nodiscard]] static constexpr T min() noexcept
		{
			return T();
		}
		[[nodiscard]] static constexpr T max() noexcept
		{
			return T();
		}
		[[nodiscard]] static constexpr T lowest() noexcept
		{
			return T();

		}
		[[nodiscard]] static constexpr T epsilon() noexcept
		{
			return T();
		}
		[[nodiscard]] static constexpr T round_error() noexcept
		{
			return T();
		}
		[[nodiscard]] static constexpr T denorm_min() noexcept
		{
			return T();
		}
		[[nodiscard]] static constexpr T infinity() noexcept
		{
			return T();
		}
		[[nodiscard]] static constexpr T quiet_NaN() noexcept
		{
			return T();
		}
		[[nodiscard]] static constexpr T signaling_NaN() noexcept
		{
			return T();
		}
	};
	template <class T>
	struct numeric_limits<const T> : public numeric_limits<T> {};

	template <class T>
	struct numeric_limits<volatile T> : public numeric_limits<T> {};

	template <class T>
	struct numeric_limits<const volatile T> : public numeric_limits<T> {};

	struct numeric_int_base : numeric_base
	{
		static constexpr bool is_bounded = true;
		static constexpr bool is_exact = true;
		static constexpr bool is_integer = true;
		static constexpr bool is_specialized = true;
		static constexpr int radix = 2;
	};

	struct numeric_float_base : numeric_base
	{
		static constexpr bool has_infinity = true;
		static constexpr bool has_quiet_NaN = true;
		static constexpr bool has_signaling_NaN = true;
		static constexpr bool is_bounded = true;
		static constexpr bool is_iec559 = true;
		static constexpr bool is_signed = true;
		static constexpr bool is_specialized = true;
	};

	template <>
	struct numeric_limits<bool> : public numeric_int_base
	{
		[[nodiscard]] static constexpr bool min() noexcept
		{
			return false;
		}
		[[nodiscard]] static constexpr bool max() noexcept
		{
			return true;
		}
		[[nodiscard]] static constexpr bool lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr bool epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr bool round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr bool denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr bool infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr bool quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr bool signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr int digist = 1;
	};
	template <>
	struct numeric_limits<char> : public numeric_int_base
	{
		[[nodiscard]] static constexpr char min() noexcept
		{
			return CHAR_MIN;
		}
		[[nodiscard]] static constexpr char max() noexcept
		{
			return CHAR_MAX;
		}
		[[nodiscard]] static constexpr char lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr char epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_signed = CHAR_MIN != 0;
		static constexpr bool is_modulo = CHAR_MIN == 0;
		static constexpr int digits = 8 - (CHAR_MIN != 0);
		static constexpr int digits10 = 2;
	};
	template <>
	struct numeric_limits<signed char> : public numeric_int_base
	{
		[[nodiscard]] static constexpr signed char min() noexcept
		{
			return SCHAR_MIN;
		}
		[[nodiscard]] static constexpr signed char max() noexcept
		{
			return SCHAR_MAX;
		}
		[[nodiscard]] static constexpr signed char lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr signed char epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr signed char round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr signed char denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr signed char infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr signed char quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr signed char signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_signed = true;
		static constexpr int digits = 7;
		static constexpr int digits10 = 2;
	};
	template <>
	struct numeric_limits<unsigned char> : public numeric_int_base
	{
		[[nodiscard]] static constexpr unsigned char min() noexcept
		{
			return UCHAR_MIN;
		}
		[[nodiscard]] static constexpr unsigned char max() noexcept
		{
			return UCHAR_MAX;
		}
		[[nodiscard]] static constexpr unsigned char lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr unsigned char epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned char round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned char denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned char infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned char quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned char signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_modulo = true;
		static constexpr int digits = 8;
		static constexpr int digits10 = 2;
	};
	template <>
	struct numeric_limits<char8_t> : public numeric_int_base
	{
		[[nodiscard]] static constexpr char8_t min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char8_t max() noexcept
		{
			return UCHAR_MAX;
		}
		[[nodiscard]] static constexpr char8_t lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr char8_t epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char8_t round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char8_t denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char8_t infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char8_t quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char8_t signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_modulo = true;
		static constexpr int digits = 8;
		static constexpr int digits10 = 2;
	};
	template <>
	struct numeric_limits<char16_t> : public numeric_int_base
	{
		[[nodiscard]] static constexpr char16_t min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char16_t max() noexcept
		{
			return USHRT_MAX;
		}
		[[nodiscard]] static constexpr char16_t lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr char16_t epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char16_t round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char16_t denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char16_t infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char16_t quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char16_t signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_modulo = true;
		static constexpr int digits = 16;
		static constexpr int digits10 = 4;
	};
	template <>
	struct numeric_limits<char32_t> : public numeric_int_base
	{
		[[nodiscard]] static constexpr char32_t min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char32_t max() noexcept
		{
			return UINT_MAX;
		}
		[[nodiscard]] static constexpr char32_t lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr char32_t epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char32_t round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char32_t denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char32_t infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char32_t quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr char32_t signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_modulo = true;
		static constexpr int digits = 32;
		static constexpr int digits10 = 9;
	};
	template <>
	struct numeric_limits<short> : public numeric_int_base
	{
		[[nodiscard]] static constexpr short min() noexcept
		{
			return SHRT_MIN;
		}
		[[nodiscard]] static constexpr short max() noexcept
		{
			return SHRT_MAX;
		}
		[[nodiscard]] static constexpr short lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr short epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr short round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr short denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr short infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr short quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr short signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_signed = true;
		static constexpr int digits = 15;
		static constexpr int digits10 = 4;
	};
	template <>
	struct numeric_limits<unsigned short> : public numeric_int_base
	{
		[[nodiscard]] static constexpr unsigned short min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned short max() noexcept
		{
			return USHRT_MAX;
		}
		[[nodiscard]] static constexpr unsigned short lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr unsigned short epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned short round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned short denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned short infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned short quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned short signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_modulo = true;
		static constexpr int digits = 16;
		static constexpr int digits10 = 4;
	};
	template <>
	struct numeric_limits<int> : public numeric_int_base
	{
		[[nodiscard]] static constexpr int min() noexcept
		{
			return INT_MIN;
		}
		[[nodiscard]] static constexpr int max() noexcept
		{
			return INT_MAX;
		}
		[[nodiscard]] static constexpr int lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr int epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr int round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr int denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr int infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr int quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr int signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_signed = true;
		static constexpr int digits = 31;
		static constexpr int digits10 = 9;
	};
	template <>
	struct numeric_limits<unsigned int> : public numeric_int_base
	{
		[[nodiscard]] static constexpr unsigned int min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned int max() noexcept
		{
			return UINT_MAX;
		}
		[[nodiscard]] static constexpr unsigned int lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr unsigned int epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned int round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned int denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned int infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned int quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned int signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_modulo = true;
		static constexpr int digits = 32;
		static constexpr int digits10 = 9;
	};
	template <>
	struct numeric_limits<long long> : public numeric_int_base
	{
		[[nodiscard]] static constexpr long long min() noexcept
		{
			return LLONG_MIN;
		}
		[[nodiscard]] static constexpr long long max() noexcept
		{
			return LLONG_MAX;
		}
		[[nodiscard]] static constexpr long long lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr long long epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr long long round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr long long denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr long long infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr long long quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr long long signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_signed = true;
		static constexpr int digits = 63;
		static constexpr int digits10 = 18;
	};
	template <>
	struct numeric_limits<unsigned long long> : public numeric_int_base
	{
		[[nodiscard]] static constexpr unsigned long long min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned long long max() noexcept
		{
			return ULLONG_MAX;
		}
		[[nodiscard]] static constexpr unsigned long long lowest() noexcept
		{
			return min();
		}
		[[nodiscard]] static constexpr unsigned long long epsilon() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned long long round_error() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned long long denorm_min() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned long long infinity() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned long long quiet_NaN() noexcept
		{
			return 0;
		}
		[[nodiscard]] static constexpr unsigned long long signaling_NaN() noexcept
		{
			return 0;
		}
		static constexpr bool is_modulo = true;
		static constexpr int digits = 64;
		static constexpr int digits10 = 19;
	};
	template <>
	struct numeric_limits<float> : public numeric_float_base
	{
		[[nodiscard]] static constexpr float min() noexcept
		{
			return FLT_MIN;
		}
		[[nodiscard]] static constexpr float max() noexcept
		{
			return FLT_MAX;
		}
		[[nodiscard]] static constexpr float lowest() noexcept
		{
			return -max();
		}
		[[nodiscard]] static constexpr float epsilon() noexcept
		{
			//return FLT_EPSILON;
			return 0;
		}
		[[nodiscard]] static constexpr float round_error() noexcept
		{
			return 0.5f;
		}
		[[nodiscard]] static constexpr float denorm_min() noexcept
		{
			//return FLT_TRUE_MIN;
			return 0;
		}
		[[nodiscard]] static constexpr float infinity() noexcept
		{
			return __builtin_huge_valf();
		}
		[[nodiscard]] static constexpr float quiet_NaN() noexcept
		{
			return __builtin_nanf("0");
		}
		[[nodiscard]] static constexpr float signaling_NaN() noexcept
		{
			return __builtin_nansf("1");
		}
		/*static constexpr int digits = FLT_MANT_DIG;
		static constexpr int digits10 = FLT_DIG;*/
		static constexpr int max_digits10 = 9;
		/*static constexpr int max_exponent = FLT_MAX_EXP;
		static constexpr int max_exponent10 = FLT_MAX_10_EXP;
		static constexpr int min_exponent = FLT_MIN_EXP;
		static constexpr int min_exponent10 = FLT_MIN_10_EXP;*/
	};
	template <>
	struct numeric_limits<double> : public numeric_float_base
	{
		[[nodiscard]] static constexpr double min() noexcept
		{
			return DBL_MIN;
		}
		[[nodiscard]] static constexpr double max() noexcept
		{
			return DBL_MAX;
		}
		[[nodiscard]] static constexpr double lowest() noexcept
		{
			return -max();
		}
		[[nodiscard]] static constexpr double epsilon() noexcept
		{
			//return DBL_EPSILON;
			return 0;
		}
		[[nodiscard]] static constexpr double round_error() noexcept
		{
			return 0.5;
		}
		[[nodiscard]] static constexpr double denorm_min() noexcept
		{
			//return DBL_TRUE_MIN;
			return 0;
		}
		[[nodiscard]] static constexpr double infinity() noexcept
		{
			return __builtin_huge_val();
		}
		[[nodiscard]] static constexpr double quiet_NaN() noexcept
		{
			return __builtin_nan("0");
		}
		[[nodiscard]] static constexpr double signaling_NaN() noexcept
		{
			return __builtin_nans("1");
		}
		/*static constexpr int digits = DBL_MANT_DIG;
		static constexpr int digits10 = DBL_DIG;*/
		static constexpr int max_digits10 = 17;
		/*static constexpr int max_exponent = DBL_MAX_EXP;
		static constexpr int max_exponent10 = DBL_MAX_10_EXP;
		static constexpr int min_exponent = DBL_MIN_EXP;
		static constexpr int min_exponent10 = DBL_MIN_10_EXP;*/
	};
}