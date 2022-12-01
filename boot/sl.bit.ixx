export module sl.bit;
import sl.type;
import sl.typetraits;
import math;
export namespace std
{
	enum class endian
	{
		little = 0,
		big = 1,
		native = little
	};

	[[nodiscard]] constexpr bool is_constant_evaluated() noexcept
	{
		return __builtin_is_constant_evaluated();
	}
	[[nodiscard]] constexpr unsigned short _Byteswap_ushort(const unsigned short value) noexcept
	{
		if (std::is_constant_evaluated()) {
			return static_cast<unsigned short>((value << 8) | (value >> 8));
		}
		else {
			return math::byteswap(value);
		}
	}

	[[nodiscard]] constexpr unsigned int _Byteswap_ulong(const unsigned int value) noexcept
	{
		if (std::is_constant_evaluated()) {
			return (value << 24) | ((value << 8) & 0x00FF'0000) | ((value >> 8) & 0x0000'FF00) | (value >> 24);
		}
		else {
			return math::byteswap(value);
		}
	}

	[[nodiscard]] constexpr unsigned long long _Byteswap_uint64(const unsigned long long value) noexcept
	{
		if (std::is_constant_evaluated()) {
			return (value << 56) | ((value << 40) & 0x00FF'0000'0000'0000) | ((value << 24) & 0x0000'FF00'0000'0000)
				| ((value << 8) & 0x0000'00FF'0000'0000) | ((value >> 8) & 0x0000'0000'FF00'0000)
				| ((value >> 24) & 0x0000'0000'00FF'0000) | ((value >> 40) & 0x0000'0000'0000'FF00) | (value >> 56);
		}
		else {
			return math::byteswap(value);
		}
	}
	template <class T>
	[[nodiscard]] constexpr T byteswap(const T value) noexcept
	{
		if constexpr (sizeof(T) == 1) {
			return value;
		}
		else if constexpr (sizeof(T) == 2) {
			return static_cast<T>(_Byteswap_ushort(static_cast<unsigned short>(value)));
		}
		else if constexpr (sizeof(T) == 4) {
			return static_cast<T>(_Byteswap_ulong(static_cast<unsigned int>(value)));
		}
		else if constexpr (sizeof(T) == 8) {
			return static_cast<T>(_Byteswap_uint64(static_cast<unsigned long long>(value)));
		}
	}
}