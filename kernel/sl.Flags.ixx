export module sl.Flags;
import sl.compare;
import sl.typetraits;

export
{
	template <typename FlagBitsType>
	struct FlagTraits
	{
		static constexpr bool isBitmask = false;
	};
	template <typename BitType>
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

	template <typename BitType>
	constexpr Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags) noexcept { return flags.operator&(bit); }

	template <typename BitType>
	constexpr Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags) noexcept { return flags.operator|(bit); }

	template <typename BitType>
	constexpr Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags) noexcept { return flags.operator^(bit); }

	template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
	inline constexpr Flags<BitType> operator&(BitType lhs, BitType rhs) noexcept
	{
		return Flags<BitType>(lhs) & rhs;
	}

	template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
	inline constexpr Flags<BitType> operator|(BitType lhs, BitType rhs) noexcept
	{
		return Flags<BitType>(lhs) | rhs;
	}

	template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
	inline constexpr Flags<BitType> operator^(BitType lhs, BitType rhs) noexcept
	{
		return Flags<BitType>(lhs) ^ rhs;
	}

	template <typename BitType, typename std::enable_if<FlagTraits<BitType>::isBitmask, bool>::type = true>
	inline constexpr Flags<BitType> operator~(BitType bit) noexcept
	{
		return ~(Flags<BitType>(bit));
	}
}