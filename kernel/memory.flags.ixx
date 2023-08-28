export module memory.flags;
import types;

export namespace memory
{
	enum MemoryFlagsBits : u8
	{
		ePRESENT = 1,
		eREAD_WRITE = 2,
		eUSER_SUPERVISOR = 4,
		eWRITE_THROUGH = 8,
		eCACHE_DISABLE = 16
	};
	//using MemoryFlags = Flags<MemoryFlagsBits>;
	//template <> struct FlagTraits<MemoryFlagsBits>
	//{
	//	static constexpr bool isBitmask = true;
	//	static constexpr Flags<MemoryFlagsBits> allFlags = MemoryFlagsBits::ePRESENT | MemoryFlagsBits::eREAD_WRITE | MemoryFlagsBits::eUSER_SUPERVISOR | MemoryFlagsBits::eWRITE_THROUGH | MemoryFlagsBits::eCACHE_DISABLE;
	//};
}