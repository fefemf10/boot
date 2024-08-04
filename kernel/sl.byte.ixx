export module sl.byte;
import sl.concepts;
export namespace std
{
	enum class byte : unsigned char {};
	template <integral T> [[nodiscard]] constexpr byte operator<<(const byte arg, const T shift) noexcept { return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(arg) << shift)); }
	template <integral T> [[nodiscard]] constexpr byte operator>>(const byte arg, const T shift) noexcept { return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(arg) >> shift)); }
	[[nodiscard]] constexpr byte operator|(const byte left, const byte right) noexcept { return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(left) | static_cast<unsigned int>(right))); }
	[[nodiscard]] constexpr byte operator&(const byte left, const byte right) noexcept { return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(left) & static_cast<unsigned int>(right))); }
	[[nodiscard]] constexpr byte operator^(const byte left, const byte right) noexcept { return static_cast<byte>(static_cast<unsigned char>(static_cast<unsigned int>(left) ^ static_cast<unsigned int>(right))); }
	[[nodiscard]] constexpr byte operator~(const byte arg) noexcept { return static_cast<byte>(static_cast<unsigned char>(~static_cast<unsigned int>(arg))); }
	template <integral T> constexpr byte& operator<<=(byte& arg, const T shift) noexcept { return arg = arg << shift; }
	template <integral T> constexpr byte& operator>>=(byte& arg, const T shift) noexcept { return arg = arg >> shift; }
	constexpr byte& operator|=(byte& left, const byte right) noexcept { return left = left | right; }
	constexpr byte& operator&=(byte& left, const byte right) noexcept { return left = left & right; }
	constexpr byte& operator^=(byte& left, const byte right) noexcept { return left = left ^ right; }
	template <integral T> [[nodiscard]] [[msvc::intrinsic]] constexpr T to_integer(const byte arg) noexcept { return static_cast<T>(arg); }
}