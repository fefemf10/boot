export module sl.Flags;
import sl.compare;
namespace std
{
	template <class T> inline constexpr bool is_enum_v = __is_enum(T);
	template <class T, bool = is_enum_v<T>>
	struct _Underlying_type
	{
		using type = __underlying_type(T);
	};
	template <class T> struct _Underlying_type<T, false> {};
	template <class T>struct underlying_type : _Underlying_type<T> {};
	template <class T> using underlying_type_t = typename _Underlying_type<T>::type;
	template <bool Test, class T = void> struct enable_if {};
	template <class T> struct enable_if<true, T> { using type = T; };
	template <bool Test, class T = void> using enable_if_t = typename enable_if<Test, T>::type;
}

export template <typename FlagBitsType>
struct FlagTraits
{
	static constexpr bool isBitmask = false;
};
export template <typename BitType>
class Flags
{
public:
	using MaskType = typename std::underlying_type<BitType>::type;

	constexpr Flags() noexcept : m_mask(0) {}
	constexpr Flags(BitType bit) noexcept : m_mask(static_cast<MaskType>(bit)) {}
	constexpr Flags(Flags<BitType> const& rhs) noexcept = default;
	constexpr explicit Flags(MaskType flags) noexcept : m_mask(flags) {}

	std::partial_ordering operator<=>(Flags<BitType> const&) const = default;

	constexpr bool operator!() const noexcept { return !m_mask; }

	constexpr Flags<BitType> operator&(Flags<BitType> const& rhs) const noexcept { return Flags<BitType>(m_mask & rhs.m_mask); }
	constexpr Flags<BitType> operator|(Flags<BitType> const& rhs) const noexcept { return Flags<BitType>(m_mask | rhs.m_mask); }
	constexpr Flags<BitType> operator^(Flags<BitType> const& rhs) const noexcept { return Flags<BitType>(m_mask ^ rhs.m_mask); }
	constexpr Flags<BitType> operator~() const noexcept { return Flags<BitType>(m_mask ^ FlagTraits<BitType>::allFlags.m_mask); }

	constexpr Flags<BitType>& operator=(Flags<BitType> const& rhs) noexcept = default;

	constexpr Flags<BitType>& operator|=(Flags<BitType> const& rhs) noexcept { m_mask |= rhs.m_mask; return *this; }
	constexpr Flags<BitType>& operator&=(Flags<BitType> const& rhs) noexcept { m_mask &= rhs.m_mask; return *this; }
	constexpr Flags<BitType>& operator^=(Flags<BitType> const& rhs) noexcept { m_mask ^= rhs.m_mask; return *this; }

	explicit constexpr operator bool() const noexcept { return !!m_mask; }
	explicit constexpr operator MaskType() const noexcept { return m_mask; }
	MaskType m_mask;
};

export template <typename BitType>
constexpr Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags) noexcept { return flags.operator&(bit); }

export template <typename BitType>
constexpr Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags) noexcept { return flags.operator|(bit); }

export template <typename BitType>
constexpr Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags) noexcept { return flags.operator^(bit); }

export template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
inline constexpr Flags<BitType> operator&(BitType lhs, BitType rhs) noexcept
{
	return Flags<BitType>(lhs) & rhs;
}

export template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
inline constexpr Flags<BitType> operator|(BitType lhs, BitType rhs) noexcept
{
	return Flags<BitType>(lhs) | rhs;
}

export template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
inline constexpr Flags<BitType> operator^(BitType lhs, BitType rhs) noexcept
{
	return Flags<BitType>(lhs) ^ rhs;
}

export template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
inline constexpr Flags<BitType> operator~(BitType bit) noexcept
{
	return ~(Flags<BitType>(bit));
}