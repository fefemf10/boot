#define BUILD0(x) case x+0:
#define BUILD1(x) BUILD0(x) case x+1:
#define BUILD2(x) BUILD1(x) case x+2:
#define BUILD3(x) BUILD2(x) case x+3:
#define BUILD4(x) BUILD3(x) case x+4:
#define BUILD5(x) BUILD4(x) case x+5:
#define BUILD6(x) BUILD5(x) case x+6:
#define BUILD7(x) BUILD6(x) case x+7:
#define BUILD8(x) BUILD7(x) case x+8:
#define BUILD9(x) BUILD8(x) case x+9:
#define BUILD10(x) BUILD9(x) case x+10:
#define BUILD11(x) BUILD10(x) case x+11:
#define BUILD12(x) BUILD11(x) case x+12:
#define BUILD13(x) BUILD12(x) case x+13:
#define BUILD14(x) BUILD13(x) case x+14:
#define BUILD15(x) BUILD14(x) case x+15:
#define BUILD16(x) BUILD15(x) case x+16:
#define BUILD_(x, i) BUILD##i(x)
#define CASEM(x, i) BUILD_(x, i)
export module translator;
import types;
import sl.utility;

export namespace translator
{
	

	enum class PREFIX : u8
	{
		LOCK = 0xF0,
		REPNE_PREPNZ = 0xF2,
		REP_REPE_PERZ = 0xF3,
		CS = 0x2E,
		SS = 0x36,
		DS = 0x3E,
		ES = 0x26,
		FS = 0x64,
		GS = 0x65,
		BRANCHNOTTAKEN = 0x2E,
		BRANCHTAKEN = 0x3E,
		OPSIZEOVERRIDE = 0x66,
		ADDRESSSIZEOVERRIDE = 0x67,
		REXF0000 = 0b0100'0000,
		REXF000B = 0b0100'0001,
		REXF00X0 = 0b0100'0010,
		REXF00XB = 0b0100'0011,
		REXF0R00 = 0b0100'0100,
		REXF0R0B = 0b0100'0101,
		REXF0RX0 = 0b0100'0110,
		REXF0RXB = 0b0100'0111,
		REXFW000 = 0b0100'1000,
		REXFW00B = 0b0100'1001,
		REXFW0X0 = 0b0100'1010,
		REXFW0XB = 0b0100'1011,
		REXFWR00 = 0b0100'1100,
		REXFWR0B = 0b0100'1101,
		REXFWRX0 = 0b0100'1110,
		REXFWRXB = 0b0100'1111
	};
	enum class OPCODE16 : u64
	{
		ADD00 = 0x00,
		ADD01 = 0x01,
		ADD02 = 0x02,
		ADD03 = 0x03,
		ADD04 = 0x04,
		ADD05 = 0x05,
	};
	enum class STATE
	{
		PREFIX = 0x0,
		OPCODE = 0x1,
		MODRM = 0x2,
		ENDOPCODE = 0x3,
		ENDJUMP = 0x4
	};
	enum class MODRM16 : u8
	{
		BXSI = 0b000,
		BXDI = 0b001,
		BPSI = 0b010,
		BPDI = 0b011,
		SI = 0b100,
		DI = 0b101,
		DISP16 = 0b110,
		BP = 0b110,
		BX = 0b111,
	};
	//enum class MODRM1601 : u8
	//{
	//	BXSID8 = 0b000,
	//	BXDID8 = 0b001,
	//	BPSID8 = 0b010,
	//	BPDID8 = 0b011,
	//	SID8 = 0b100,
	//	DID8 = 0b101,
	//	BPD8 = 0b110,
	//	BXD8 = 0b111,
	//};
	//enum class MODRM1610 : u8
	//{
	//	BXSID16 = 0b000,
	//	BXDID16 = 0b001,
	//	BPSID16 = 0b010,
	//	BPDID16 = 0b011,
	//	SID16 = 0b100,
	//	DID16 = 0b101,
	//	BPD16 = 0b110,
	//	BXD16 = 0b111,
	//};
	enum class MODRM64 : u8
	{
		RAX = 0b000,
		RCX = 0b001,
		RDX = 0b010,
		RBX = 0b011,
		SIB = 0b100,
		RSP = 0b100,
		RIP = 0b101,
		RBP = 0b101,
		RSI = 0b110,
		RDI = 0b111,
	};
	//enum class MODRM6401 : u8
	//{
	//	RAXD8 = 0b000,
	//	RCXD8 = 0b001,
	//	RDXD8 = 0b010,
	//	RBXD8 = 0b011,
	//	SIBD8 = 0b100,
	//	RBPD8 = 0b101,
	//	RSID8 = 0b110,
	//	RDID8 = 0b111,
	//};
	//enum class MODRM6410 : u8
	//{
	//	RAXD32 = 0b000,
	//	RCXD32 = 0b001,
	//	RDXD32 = 0b010,
	//	RBXD32 = 0b011,
	//	SIBD32 = 0b100,
	//	RBPD32 = 0b101,
	//	RSID32 = 0b110,
	//	RDID32 = 0b111,
	//};

	enum class MODREG : u8
	{
		AL = 0b000,
		AX = 0b000,
		EAX = 0b000,
		RAX = 0b000,
		CL = 0b001,
		CX = 0b001,
		ECX = 0b001,
		RCX = 0b001,
		DL = 0b010,
		DX = 0b010,
		EDX = 0b010,
		RDX = 0b010,
		BL = 0b011,
		BX = 0b011,
		EBX = 0b011,
		RBX = 0b011,
		AH = 0b100,
		SPL = 0b100,
		SP = 0b100,
		ESP = 0b100,
		RSP = 0b100,
		CH = 0b101,
		BPL = 0b101,
		BP = 0b101,
		EBP = 0b101,
		RBP = 0b101,
		DH = 0b110,
		SIL = 0b110,
		SI = 0b110,
		ESI = 0b110,
		RSI = 0b110,
		BH = 0b111,
		DIL = 0b111,
		DI = 0b111,
		EDI = 0b111,
		RDI = 0b111,
	};

	enum class MODSIB64 : u8
	{
		RAX = 0b000,
		RCX = 0b001,
		RDX = 0b010,
		RBX = 0b011,
		NONE = 0b100,
		RBP = 0b101,
		RSI = 0b110,
		RDI = 0b111,
	};
	enum class MODSIBBASE64 : u8
	{
		RAX = 0b000,
		RCX = 0b001,
		RDX = 0b010,
		RBX = 0b011,
		RSP = 0b100,
		NOTE = 0b101,
		RSI = 0b110,
		RDI = 0b111,
	};

	constexpr MODSIBBASE64 operator&(MODSIBBASE64 x, MODSIBBASE64 y) { return static_cast<MODSIBBASE64>(std::to_underlying(x) & std::to_underlying(y)); };
	constexpr MODSIBBASE64 operator|(MODSIBBASE64 x, MODSIBBASE64 y) { return static_cast<MODSIBBASE64>(std::to_underlying(x) | std::to_underlying(y)); };
	constexpr MODSIBBASE64 operator^(MODSIBBASE64 x, MODSIBBASE64 y) { return static_cast<MODSIBBASE64>(std::to_underlying(x) ^ std::to_underlying(y)); };
	constexpr MODSIBBASE64 operator~(MODSIBBASE64 x) { return static_cast<MODSIBBASE64>(~std::to_underlying(x)); };
	constexpr MODSIBBASE64 operator&(MODSIBBASE64 x, std::underlying_type<MODSIBBASE64>::type y) { return static_cast<MODSIBBASE64>(std::to_underlying(x) & y); };
	constexpr MODSIBBASE64 operator|(MODSIBBASE64 x, std::underlying_type<MODSIBBASE64>::type y) { return static_cast<MODSIBBASE64>(std::to_underlying(x) | y); };
	constexpr MODSIBBASE64 operator^(MODSIBBASE64 x, std::underlying_type<MODSIBBASE64>::type y) { return static_cast<MODSIBBASE64>(std::to_underlying(x) ^ y); };
	constexpr MODSIBBASE64 operator&(std::underlying_type<MODSIBBASE64>::type x, MODSIBBASE64 y) { return static_cast<MODSIBBASE64>(x & std::to_underlying(y)); };
	constexpr MODSIBBASE64 operator|(std::underlying_type<MODSIBBASE64>::type x, MODSIBBASE64 y) { return static_cast<MODSIBBASE64>(x | std::to_underlying(y)); };
	constexpr MODSIBBASE64 operator^(std::underlying_type<MODSIBBASE64>::type x, MODSIBBASE64 y) { return static_cast<MODSIBBASE64>(x ^ std::to_underlying(y)); };

	constexpr MODSIB64 operator&(MODSIB64 x, MODSIB64 y) { return static_cast<MODSIB64>(std::to_underlying(x) & std::to_underlying(y)); };
	constexpr MODSIB64 operator|(MODSIB64 x, MODSIB64 y) { return static_cast<MODSIB64>(std::to_underlying(x) | std::to_underlying(y)); };
	constexpr MODSIB64 operator^(MODSIB64 x, MODSIB64 y) { return static_cast<MODSIB64>(std::to_underlying(x) ^ std::to_underlying(y)); };
	constexpr MODSIB64 operator~(MODSIB64 x) { return static_cast<MODSIB64>(~std::to_underlying(x)); };
	constexpr MODSIB64 operator&(MODSIB64 x, std::underlying_type<MODSIB64>::type y) { return static_cast<MODSIB64>(std::to_underlying(x) & y); };
	constexpr MODSIB64 operator|(MODSIB64 x, std::underlying_type<MODSIB64>::type y) { return static_cast<MODSIB64>(std::to_underlying(x) | y); };
	constexpr MODSIB64 operator^(MODSIB64 x, std::underlying_type<MODSIB64>::type y) { return static_cast<MODSIB64>(std::to_underlying(x) ^ y); };
	constexpr MODSIB64 operator&(std::underlying_type<MODSIB64>::type x, MODSIB64 y) { return static_cast<MODSIB64>(x & std::to_underlying(y)); };
	constexpr MODSIB64 operator|(std::underlying_type<MODSIB64>::type x, MODSIB64 y) { return static_cast<MODSIB64>(x | std::to_underlying(y)); };
	constexpr MODSIB64 operator^(std::underlying_type<MODSIB64>::type x, MODSIB64 y) { return static_cast<MODSIB64>(x ^ std::to_underlying(y)); };

	constexpr PREFIX operator&(PREFIX x, PREFIX y) { return static_cast<PREFIX>(std::to_underlying(x) & std::to_underlying(y)); };
	constexpr PREFIX operator|(PREFIX x, PREFIX y) { return static_cast<PREFIX>(std::to_underlying(x) | std::to_underlying(y)); };
	constexpr PREFIX operator^(PREFIX x, PREFIX y) { return static_cast<PREFIX>(std::to_underlying(x) ^ std::to_underlying(y)); };
	constexpr PREFIX operator~(PREFIX x) { return static_cast<PREFIX>(~std::to_underlying(x)); };
	constexpr PREFIX operator&(PREFIX x, std::underlying_type<PREFIX>::type y) { return static_cast<PREFIX>(std::to_underlying(x) & y); };
	constexpr PREFIX operator|(PREFIX x, std::underlying_type<PREFIX>::type y) { return static_cast<PREFIX>(std::to_underlying(x) | y); };
	constexpr PREFIX operator^(PREFIX x, std::underlying_type<PREFIX>::type y) { return static_cast<PREFIX>(std::to_underlying(x) ^ y); };
	constexpr PREFIX operator&(std::underlying_type<PREFIX>::type x, PREFIX y) { return static_cast<PREFIX>(x & std::to_underlying(y)); };
	constexpr PREFIX operator|(std::underlying_type<PREFIX>::type x, PREFIX y) { return static_cast<PREFIX>(x | std::to_underlying(y)); };
	constexpr PREFIX operator^(std::underlying_type<PREFIX>::type x, PREFIX y) { return static_cast<PREFIX>(x ^ std::to_underlying(y)); };

	constexpr OPCODE16 operator&(OPCODE16 x, OPCODE16 y) { return static_cast<OPCODE16>(std::to_underlying(x) & std::to_underlying(y)); };
	constexpr OPCODE16 operator|(OPCODE16 x, OPCODE16 y) { return static_cast<OPCODE16>(std::to_underlying(x) | std::to_underlying(y)); };
	constexpr OPCODE16 operator^(OPCODE16 x, OPCODE16 y) { return static_cast<OPCODE16>(std::to_underlying(x) ^ std::to_underlying(y)); };
	constexpr OPCODE16 operator~(OPCODE16 x) { return static_cast<OPCODE16>(~std::to_underlying(x)); };
	constexpr OPCODE16 operator&(OPCODE16 x, std::underlying_type<OPCODE16>::type y) { return static_cast<OPCODE16>(std::to_underlying(x) & y); };
	constexpr OPCODE16 operator|(OPCODE16 x, std::underlying_type<OPCODE16>::type y) { return static_cast<OPCODE16>(std::to_underlying(x) | y); };
	constexpr OPCODE16 operator^(OPCODE16 x, std::underlying_type<OPCODE16>::type y) { return static_cast<OPCODE16>(std::to_underlying(x) ^ y); };
	constexpr OPCODE16 operator&(std::underlying_type<OPCODE16>::type x, OPCODE16 y) { return static_cast<OPCODE16>(x & std::to_underlying(y)); };
	constexpr OPCODE16 operator|(std::underlying_type<OPCODE16>::type x, OPCODE16 y) { return static_cast<OPCODE16>(x | std::to_underlying(y)); };
	constexpr OPCODE16 operator^(std::underlying_type<OPCODE16>::type x, OPCODE16 y) { return static_cast<OPCODE16>(x ^ std::to_underlying(y)); };

	constexpr MODRM16 operator&(MODRM16 x, MODRM16 y) { return static_cast<MODRM16>(std::to_underlying(x) & std::to_underlying(y)); };
	constexpr MODRM16 operator|(MODRM16 x, MODRM16 y) { return static_cast<MODRM16>(std::to_underlying(x) | std::to_underlying(y)); };
	constexpr MODRM16 operator^(MODRM16 x, MODRM16 y) { return static_cast<MODRM16>(std::to_underlying(x) ^ std::to_underlying(y)); };
	constexpr MODRM16 operator~(MODRM16 x) { return static_cast<MODRM16>(~std::to_underlying(x)); };
	constexpr MODRM16 operator&(MODRM16 x, std::underlying_type<MODRM16>::type y) { return static_cast<MODRM16>(std::to_underlying(x) & y); };
	constexpr MODRM16 operator|(MODRM16 x, std::underlying_type<MODRM16>::type y) { return static_cast<MODRM16>(std::to_underlying(x) | y); };
	constexpr MODRM16 operator^(MODRM16 x, std::underlying_type<MODRM16>::type y) { return static_cast<MODRM16>(std::to_underlying(x) ^ y); };
	constexpr MODRM16 operator&(std::underlying_type<MODRM16>::type x, MODRM16 y) { return static_cast<MODRM16>(x & std::to_underlying(y)); };
	constexpr MODRM16 operator|(std::underlying_type<MODRM16>::type x, MODRM16 y) { return static_cast<MODRM16>(x | std::to_underlying(y)); };
	constexpr MODRM16 operator^(std::underlying_type<MODRM16>::type x, MODRM16 y) { return static_cast<MODRM16>(x ^ std::to_underlying(y)); };

	//constexpr MODRM1601 operator&(MODRM1601 x, MODRM1601 y) { return static_cast<MODRM1601>(std::to_underlying(x) & std::to_underlying(y)); };
	//constexpr MODRM1601 operator|(MODRM1601 x, MODRM1601 y) { return static_cast<MODRM1601>(std::to_underlying(x) | std::to_underlying(y)); };
	//constexpr MODRM1601 operator^(MODRM1601 x, MODRM1601 y) { return static_cast<MODRM1601>(std::to_underlying(x) ^ std::to_underlying(y)); };
	//constexpr MODRM1601 operator~(MODRM1601 x) { return static_cast<MODRM1601>(~std::to_underlying(x)); };
	//constexpr MODRM1601 operator&(MODRM1601 x, std::underlying_type<MODRM1601>::type y) { return static_cast<MODRM1601>(std::to_underlying(x) & y); };
	//constexpr MODRM1601 operator|(MODRM1601 x, std::underlying_type<MODRM1601>::type y) { return static_cast<MODRM1601>(std::to_underlying(x) | y); };
	//constexpr MODRM1601 operator^(MODRM1601 x, std::underlying_type<MODRM1601>::type y) { return static_cast<MODRM1601>(std::to_underlying(x) ^ y); };
	//constexpr MODRM1601 operator&(std::underlying_type<MODRM1601>::type x, MODRM1601 y) { return static_cast<MODRM1601>(x & std::to_underlying(y)); };
	//constexpr MODRM1601 operator|(std::underlying_type<MODRM1601>::type x, MODRM1601 y) { return static_cast<MODRM1601>(x | std::to_underlying(y)); };
	//constexpr MODRM1601 operator^(std::underlying_type<MODRM1601>::type x, MODRM1601 y) { return static_cast<MODRM1601>(x ^ std::to_underlying(y)); };
	//
	//constexpr MODRM1610 operator&(MODRM1610 x, MODRM1610 y) { return static_cast<MODRM1610>(std::to_underlying(x) & std::to_underlying(y)); };
	//constexpr MODRM1610 operator|(MODRM1610 x, MODRM1610 y) { return static_cast<MODRM1610>(std::to_underlying(x) | std::to_underlying(y)); };
	//constexpr MODRM1610 operator^(MODRM1610 x, MODRM1610 y) { return static_cast<MODRM1610>(std::to_underlying(x) ^ std::to_underlying(y)); };
	//constexpr MODRM1610 operator~(MODRM1610 x) { return static_cast<MODRM1610>(~std::to_underlying(x)); };
	//constexpr MODRM1610 operator&(MODRM1610 x, std::underlying_type<MODRM1610>::type y) { return static_cast<MODRM1610>(std::to_underlying(x) & y); };
	//constexpr MODRM1610 operator|(MODRM1610 x, std::underlying_type<MODRM1610>::type y) { return static_cast<MODRM1610>(std::to_underlying(x) | y); };
	//constexpr MODRM1610 operator^(MODRM1610 x, std::underlying_type<MODRM1610>::type y) { return static_cast<MODRM1610>(std::to_underlying(x) ^ y); };
	//constexpr MODRM1610 operator&(std::underlying_type<MODRM1610>::type x, MODRM1610 y) { return static_cast<MODRM1610>(x & std::to_underlying(y)); };
	//constexpr MODRM1610 operator|(std::underlying_type<MODRM1610>::type x, MODRM1610 y) { return static_cast<MODRM1610>(x | std::to_underlying(y)); };
	//constexpr MODRM1610 operator^(std::underlying_type<MODRM1610>::type x, MODRM1610 y) { return static_cast<MODRM1610>(x ^ std::to_underlying(y)); };

	constexpr MODRM64 operator&(MODRM64 x, MODRM64 y) { return static_cast<MODRM64>(std::to_underlying(x) & std::to_underlying(y)); };
	constexpr MODRM64 operator|(MODRM64 x, MODRM64 y) { return static_cast<MODRM64>(std::to_underlying(x) | std::to_underlying(y)); };
	constexpr MODRM64 operator^(MODRM64 x, MODRM64 y) { return static_cast<MODRM64>(std::to_underlying(x) ^ std::to_underlying(y)); };
	constexpr MODRM64 operator~(MODRM64 x) { return static_cast<MODRM64>(~std::to_underlying(x)); };
	constexpr MODRM64 operator&(MODRM64 x, std::underlying_type<MODRM64>::type y) { return static_cast<MODRM64>(std::to_underlying(x) & y); };
	constexpr MODRM64 operator|(MODRM64 x, std::underlying_type<MODRM64>::type y) { return static_cast<MODRM64>(std::to_underlying(x) | y); };
	constexpr MODRM64 operator^(MODRM64 x, std::underlying_type<MODRM64>::type y) { return static_cast<MODRM64>(std::to_underlying(x) ^ y); };
	constexpr MODRM64 operator&(std::underlying_type<MODRM64>::type x, MODRM64 y) { return static_cast<MODRM64>(x & std::to_underlying(y)); };
	constexpr MODRM64 operator|(std::underlying_type<MODRM64>::type x, MODRM64 y) { return static_cast<MODRM64>(x | std::to_underlying(y)); };
	constexpr MODRM64 operator^(std::underlying_type<MODRM64>::type x, MODRM64 y) { return static_cast<MODRM64>(x ^ std::to_underlying(y)); };

	//constexpr MODRM6401 operator&(MODRM6401 x, MODRM1601 y) { return static_cast<MODRM6401>(std::to_underlying(x) & std::to_underlying(y)); };
	//constexpr MODRM6401 operator|(MODRM6401 x, MODRM1601 y) { return static_cast<MODRM6401>(std::to_underlying(x) | std::to_underlying(y)); };
	//constexpr MODRM6401 operator^(MODRM6401 x, MODRM1601 y) { return static_cast<MODRM6401>(std::to_underlying(x) ^ std::to_underlying(y)); };
	//constexpr MODRM6401 operator~(MODRM6401 x) { return static_cast<MODRM6401>(~std::to_underlying(x)); };
	//constexpr MODRM6401 operator&(MODRM6401 x, std::underlying_type<MODRM6401>::type y) { return static_cast<MODRM6401>(std::to_underlying(x) & y); };
	//constexpr MODRM6401 operator|(MODRM6401 x, std::underlying_type<MODRM6401>::type y) { return static_cast<MODRM6401>(std::to_underlying(x) | y); };
	//constexpr MODRM6401 operator^(MODRM6401 x, std::underlying_type<MODRM6401>::type y) { return static_cast<MODRM6401>(std::to_underlying(x) ^ y); };
	//constexpr MODRM6401 operator&(std::underlying_type<MODRM6401>::type x, MODRM6401 y) { return static_cast<MODRM6401>(x & std::to_underlying(y)); };
	//constexpr MODRM6401 operator|(std::underlying_type<MODRM6401>::type x, MODRM6401 y) { return static_cast<MODRM6401>(x | std::to_underlying(y)); };
	//constexpr MODRM6401 operator^(std::underlying_type<MODRM6401>::type x, MODRM6401 y) { return static_cast<MODRM6401>(x ^ std::to_underlying(y)); };
	//
	//constexpr MODRM6410 operator&(MODRM6410 x, MODRM1610 y) { return static_cast<MODRM6410>(std::to_underlying(x) & std::to_underlying(y)); };
	//constexpr MODRM6410 operator|(MODRM6410 x, MODRM1610 y) { return static_cast<MODRM6410>(std::to_underlying(x) | std::to_underlying(y)); };
	//constexpr MODRM6410 operator^(MODRM6410 x, MODRM1610 y) { return static_cast<MODRM6410>(std::to_underlying(x) ^ std::to_underlying(y)); };
	//constexpr MODRM6410 operator~(MODRM6410 x) { return static_cast<MODRM6410>(~std::to_underlying(x)); };
	//constexpr MODRM6410 operator&(MODRM6410 x, std::underlying_type<MODRM6410>::type y) { return static_cast<MODRM6410>(std::to_underlying(x) & y); };
	//constexpr MODRM6410 operator|(MODRM6410 x, std::underlying_type<MODRM6410>::type y) { return static_cast<MODRM6410>(std::to_underlying(x) | y); };
	//constexpr MODRM6410 operator^(MODRM6410 x, std::underlying_type<MODRM6410>::type y) { return static_cast<MODRM6410>(std::to_underlying(x) ^ y); };
	//constexpr MODRM6410 operator&(std::underlying_type<MODRM6410>::type x, MODRM6410 y) { return static_cast<MODRM6410>(x & std::to_underlying(y)); };
	//constexpr MODRM6410 operator|(std::underlying_type<MODRM6410>::type x, MODRM6410 y) { return static_cast<MODRM6410>(x | std::to_underlying(y)); };
	//constexpr MODRM6410 operator^(std::underlying_type<MODRM6410>::type x, MODRM6410 y) { return static_cast<MODRM6410>(x ^ std::to_underlying(y)); };

	constexpr MODREG operator&(MODREG x, MODREG y) { return static_cast<MODREG>(std::to_underlying(x) & std::to_underlying(y)); };
	constexpr MODREG operator|(MODREG x, MODREG y) { return static_cast<MODREG>(std::to_underlying(x) | std::to_underlying(y)); };
	constexpr MODREG operator^(MODREG x, MODREG y) { return static_cast<MODREG>(std::to_underlying(x) ^ std::to_underlying(y)); };
	constexpr MODREG operator~(MODREG x) { return static_cast<MODREG>(~std::to_underlying(x)); };
	constexpr MODREG operator&(MODREG x, std::underlying_type<MODREG>::type y) { return static_cast<MODREG>(std::to_underlying(x) & y); };
	constexpr MODREG operator|(MODREG x, std::underlying_type<MODREG>::type y) { return static_cast<MODREG>(std::to_underlying(x) | y); };
	constexpr MODREG operator^(MODREG x, std::underlying_type<MODREG>::type y) { return static_cast<MODREG>(std::to_underlying(x) ^ y); };
	constexpr MODREG operator&(std::underlying_type<MODREG>::type x, MODREG y) { return static_cast<MODREG>(x & std::to_underlying(y)); };
	constexpr MODREG operator|(std::underlying_type<MODREG>::type x, MODREG y) { return static_cast<MODREG>(x | std::to_underlying(y)); };
	constexpr MODREG operator^(std::underlying_type<MODREG>::type x, MODREG y) { return static_cast<MODREG>(x ^ std::to_underlying(y)); };
}

export namespace translator
{
	
	constexpr u8 putRM(u8 mod, MODREG reg, MODRM64 rm)
	{
		return (mod & 0b11) << 6 | (std::to_underlying(reg) & 0b111) << 3 | (std::to_underlying(rm) & 0b111);

	}
	constexpr u8 putSIB(u8 ss, MODSIB64 index, MODSIBBASE64 base)
	{
		return (ss & 0b11) << 6 | (std::to_underlying(index) & 0b111) << 3 | (std::to_underlying(base) & 0b111);
	}
	int search(int* cmd, int size, int value)
	{
		for (int i = 0; i < size; i++)
			if (cmd[i] == value)
				return i;
		return -1;
	}
	i64 functionMap64[1]{};
	i64 functionMap16[1]{};
	i64 functionMapOffset{};

	void translator(void* address, void* destination, u64& size)
	{
		functionMap16[functionMapOffset] = reinterpret_cast<i64>(address);
		functionMap64[functionMapOffset] = reinterpret_cast<i64>(destination);
		++functionMapOffset;
		u8* src = reinterpret_cast<u8*>(address);
		u8* dst = reinterpret_cast<u8*>(destination);
		i64 srcoffset = 0;
		i64 dstoffset = 0;
		u8 operandSize = 8;
		u8 addressSize = 8;
		bool repeatPrefix = false;
		STATE st = STATE::PREFIX;
		bool ret = false;
		int cmd16[250]{};
		int cmd64[250]{};
		int cmdJump[100]{};
		int cmdOffset{};
		int cmdJumpOffset{};
		cmd16[cmdOffset] = srcoffset;
		cmd64[cmdOffset] = dstoffset;
		++cmdOffset;
		int counter = 256;
		while (!ret && counter != 0)
		{
			switch (st)
			{
			case STATE::PREFIX:
			PREFIX_LABEL:
				switch (static_cast<PREFIX>(src[srcoffset]))
				{
				case PREFIX::OPSIZEOVERRIDE:
					st = STATE::PREFIX;
					operandSize = 32;
					++srcoffset;
					goto PREFIX_LABEL;
					break;
				case PREFIX::ADDRESSSIZEOVERRIDE:
					st = STATE::PREFIX;
					addressSize = 32;
					++srcoffset;
					goto PREFIX_LABEL;
					break;
				case PREFIX::REPNE_PREPNZ:
				case PREFIX::REP_REPE_PERZ:
					st = STATE::PREFIX;
					repeatPrefix = true;
					++srcoffset;
					goto PREFIX_LABEL;
					break;
				default:
					st = STATE::OPCODE;
					goto OPCODE_LABEL;
					break;
				}
				break;
			case STATE::OPCODE:
			OPCODE_LABEL:
				if ((src[srcoffset] >= 0 && src[srcoffset] <= 0x3D && ((src[srcoffset] - 1) % 8 == 0 || (src[srcoffset] - 3) % 8 == 0)) || src[srcoffset] >= 0x50 && src[srcoffset] <= 0x5F ||
					src[srcoffset] == 0x69 || src[srcoffset] == 0x6B || src[srcoffset] == 0x81 ||
					src[srcoffset] == 0x83 ||
					src[srcoffset] == 0x85 || src[srcoffset] == 0x87 || src[srcoffset] == 0x89 || src[srcoffset] == 0x8B ||
					src[srcoffset] >= 0x91 && src[srcoffset] <= 0x97 || src[srcoffset] == 0x98 || src[srcoffset] == 0x99 ||
					src[srcoffset] == 0xA1 || src[srcoffset] == 0xA3 || src[srcoffset] == 0xA5 || src[srcoffset] == 0xA7 ||
					src[srcoffset] == 0xA9 || src[srcoffset] == 0xAB || src[srcoffset] == 0xAD || src[srcoffset] == 0xAF ||
					src[srcoffset] >= 0xB8 && src[srcoffset] <= 0xBF || src[srcoffset] == 0xC1 || src[srcoffset] == 0xD1 ||
					src[srcoffset] == 0xD3 || src[srcoffset] == 0xC7 || src[srcoffset] == 0xE5 || src[srcoffset] == 0xE7 ||
					src[srcoffset] == 0xE8 || src[srcoffset] == 0xF7 || src[srcoffset] == 0x60 || src[srcoffset] == 0x61 ||
					src[srcoffset] == 0xE9)
				{
					if (operandSize != 32)
						operandSize = 16;
				}
				if (src[srcoffset] >= 0xA0 && src[srcoffset] <= 0xA3)
				{
					if (addressSize != 32)
						addressSize = 16;
				}
				switch (src[srcoffset])
				{
					//OPCODE + MODRM
					CASEM(0x00, 3)
						CASEM(0x08, 3)
						CASEM(0x10, 3)
						CASEM(0x18, 3)
						CASEM(0x20, 3)
						CASEM(0x28, 3)
						CASEM(0x30, 3)
						CASEM(0x38, 3)
						CASEM(0x69, 0)
						CASEM(0x6B, 0)
						CASEM(0x80, 1)
						CASEM(0x83, 0)
						CASEM(0x84, 1)
						CASEM(0x86, 1)
						CASEM(0x88, 1)
						CASEM(0x8A, 1)
						CASEM(0xC0, 1)
						CASEM(0xC6, 1)
						CASEM(0xD2, 1)
						CASEM(0xF6, 1)
					{
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						st = STATE::MODRM;
						break;
					}
					//OPCODE
					CASEM(0x50, 15)
						CASEM(0x91, 6)
						CASEM(0x98, 1)
						CASEM(0x9B, 0)
						CASEM(0x9C, 3)
						CASEM(0xA4, 3)
						CASEM(0xAA, 5)
						CASEM(0xC9, 0)
						CASEM(0xCC, 0)
						CASEM(0xF1, 0)
						CASEM(0xF4, 1)
						CASEM(0xF8, 5)
					{
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						st = STATE::ENDOPCODE;
						break;
					}
					//PUSHA
					CASEM(0x60, 0)
					{
						++srcoffset;
						for (size_t i = 0; i < 8; i++)
						{
							if (operandSize == 16)
								dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
							dst[dstoffset++] = 0x50 + i;
						}
						st = STATE::ENDOPCODE;
						break;
					}
					//POPA
					CASEM(0x61, 0)
					{
						++srcoffset;
						for (size_t i = 0; i < 8; i++)
						{
							if (operandSize == 16)
								dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
							dst[dstoffset++] = 0x5F - i;
						}
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE FOR PUSH ES
					CASEM(0x06, 0)
					{
						++srcoffset;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x8C;
						dst[dstoffset++] = 0xC0;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x50;
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE FOR POP ES
					CASEM(0x07, 0)
					{
						++srcoffset;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x58;
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x8E;
						dst[dstoffset++] = 0xC0;
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE FOR PUSH SS
					CASEM(0x16, 0)
					{
						++srcoffset;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x8C;
						dst[dstoffset++] = 0xD2;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x52;
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE FOR POP SS
					CASEM(0x17, 0)
					{
						++srcoffset;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x5A;
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x8E;
						dst[dstoffset++] = 0xD2;
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE FOR PUSH DS
					CASEM(0x1E, 0)
					{
						++srcoffset;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x8C;
						dst[dstoffset++] = 0xDB;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x53;
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE FOR POP DS
					CASEM(0x1F, 0)
					{
						++srcoffset;
						dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x5B;
						dst[dstoffset++] = std::to_underlying(PREFIX::REXF000B);
						dst[dstoffset++] = 0x8E;
						dst[dstoffset++] = 0xDB;
						st = STATE::ENDOPCODE;
						break;
					}
					CASEM(0xCB, 0)
					{
						dst[dstoffset++] = std::to_underlying(PREFIX::REXFW000);
						dst[dstoffset++] = src[srcoffset++];
						st = STATE::ENDOPCODE;
						break;
					}
					CASEM(0xCF, 0)
					{
						if (operandSize == 8)
							dst[dstoffset++] = std::to_underlying(PREFIX::REXFW000);
						dst[dstoffset++] = src[srcoffset++];
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE + IMM16
					CASEM(0x68, 0)
						CASEM(0xA0, 3)
					{
						if (addressSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::ADDRESSSIZEOVERRIDE);
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 4;
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE + IMM16 or IMM32
					CASEM(0x00 + 5, 0)
						CASEM(0x08 + 5, 0)
						CASEM(0x10 + 5, 0)
						CASEM(0x18 + 5, 0)
						CASEM(0x20 + 5, 0)
						CASEM(0x28 + 5, 0)
						CASEM(0x30 + 5, 0)
						CASEM(0x38 + 5, 0)
						CASEM(0xA9, 0)
						CASEM(0xB8, 7)
						CASEM(0xC2, 0)
					{
						if (addressSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::ADDRESSSIZEOVERRIDE);
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						if (operandSize == 32)
						{
							*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
							srcoffset += 4;
							dstoffset += 4;
						}
						else
						{
							*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
							srcoffset += 2;
							dstoffset += 2;
						}
						st = STATE::ENDOPCODE;
						break;
					}
					//SPECIALLY FOR CALL
					CASEM(0x70, 15)
						CASEM(0xEB, 0)
					{
						dst[dstoffset++] = src[srcoffset++];
						dst[dstoffset++] = src[srcoffset++];
						st = STATE::ENDJUMP;
						break;
					}
					//SPECIALLY FOR CALL
					CASEM(0xE8, 0)
					{
						if (addressSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::ADDRESSSIZEOVERRIDE);
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						if (operandSize == 32)
						{
							*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
							srcoffset += 4;
							dstoffset += 4;
						}
						else
						{
							*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
							srcoffset += 2;
							dstoffset += 2;
						}
						st = STATE::ENDJUMP;
						break;
					}
					CASEM(0xE9, 0)
					{
						if (addressSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::ADDRESSSIZEOVERRIDE);
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						if (operandSize == 32)
						{
							*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
							srcoffset += 4;
							dstoffset += 4;
						}
						else
						{
							*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
							srcoffset += 2;
							dstoffset += 2;
						}
						st = STATE::ENDJUMP;
						break;
					}
					CASEM(0xCA, 0)
					{
						dst[dstoffset++] = std::to_underlying(PREFIX::REXFW000);
						dst[dstoffset++] = src[srcoffset++];
						*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 2;
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE + IMM16 + IMM8
					CASEM(0xC8, 0)
					{
						if (addressSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::ADDRESSSIZEOVERRIDE);
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 2;
						dst[dstoffset++] = src[srcoffset++];
						st = STATE::ENDOPCODE;
						break;
					}
					//OPCODE + IMM8
					CASEM(0x00 + 4, 0)
						CASEM(0x08 + 4, 0)
						CASEM(0x10 + 4, 0)
						CASEM(0x18 + 4, 0)
						CASEM(0x20 + 4, 0)
						CASEM(0x28 + 4, 0)
						CASEM(0x30 + 4, 0)
						CASEM(0x38 + 4, 0)
						CASEM(0x6A, 0)
						CASEM(0xA8, 0)
						CASEM(0xB0, 7)
						CASEM(0xD0, 1)
						CASEM(0xCD, 0)
						CASEM(0xE0, 2)
						CASEM(0xE4, 3)
					{
						if (addressSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::ADDRESSSIZEOVERRIDE);
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						dst[dstoffset++] = src[srcoffset++];
						st = STATE::ENDOPCODE;
						break;
					}
					//2 BYTE OPCODE
					CASEM(0x0F, 0)
					{
						if (src[srcoffset + 1] >= 0x80 && src[srcoffset + 1] <= 0x8F)
						{
							if (operandSize != 32)
								operandSize = 16;
						}
						if (addressSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::ADDRESSSIZEOVERRIDE);
						if (operandSize == 16)
							dst[dstoffset++] = std::to_underlying(PREFIX::OPSIZEOVERRIDE);
						dst[dstoffset++] = src[srcoffset++];
						switch (src[srcoffset])
						{
							//OPCODE
							CASEM(0xA0, 2)
							CASEM(0xA8, 1)
							{
								dst[dstoffset++] = src[srcoffset++];
								st = STATE::ENDOPCODE;
								break;
							}
							//OPCODE + IMM8
							CASEM(0x90, 15)
							{
								dst[dstoffset++] = src[srcoffset++];
								dst[dstoffset++] = src[srcoffset++];
								st = STATE::ENDOPCODE;
								break;
							}
							//JUMP REL16
							CASEM(0x80, 15)
							{
								dst[dstoffset++] = src[srcoffset++];
								if (operandSize == 32)
								{
									*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
									srcoffset += 4;
									dstoffset += 4;
								}
								else
								{
									*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
									srcoffset += 2;
									dstoffset += 2;
								}
								st = STATE::ENDJUMP;
								break;
							}
						}
						break;
					}
					//OPCODE STOP
				case 0xC3:
				{
					dst[dstoffset++] = src[srcoffset++];
					ret = true;
					st = STATE::ENDOPCODE;
					break;
				}
				case 0x90:
				{
					dst[dstoffset++] = src[srcoffset++];
					st = STATE::ENDOPCODE;
					break;
				}
				}
				break;
			case STATE::MODRM:
			{
				u8 mrm = src[srcoffset++];
				u8 mrmmod = mrm >> 6;
				MODREG mrmreg = static_cast<MODREG>((mrm >> 3) & 0x07);
				u8 mrmrm = mrm & 0x07;
				//SPECIAL FOR MOV R/M32 IMM32
				if (src[srcoffset - 2] == 0xC7)
				{
					if (operandSize == 32)
					{
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
						dst[dstoffset++] = putSIB(0, MODSIB64::NONE, MODSIBBASE64::NOTE);
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
						srcoffset += 4;
						dstoffset += 4;
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
						srcoffset += 4;
						dstoffset += 4;
						st = STATE::ENDOPCODE;
						break;
					}
				}
				switch (mrmmod)
				{
				case 0b00:
					switch (static_cast<MODRM16>(mrmrm))
					{
					case MODRM16::BXSI:
					case MODRM16::BXDI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
						dst[dstoffset++] = putSIB(0, mrmrm | MODSIB64::RSI, MODSIBBASE64::RBX);
						break;
					case MODRM16::BPSI:
					case MODRM16::BPDI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
						dst[dstoffset++] = putSIB(0, MODSIB64::RBP, mrmrm | MODSIBBASE64::RSI);
						break;
					case MODRM16::SI:
					case MODRM16::DI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, mrmrm | MODRM64::RSI);
						break;
					case MODRM16::DISP16:
					{
						u8 opcode = src[srcoffset - 2];
						if (addressSize == 32)
						{
							dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
							dst[dstoffset++] = putSIB(0, MODSIB64::NONE, MODSIBBASE64::NOTE);
							*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
							srcoffset += 4;
							dstoffset += 4;
						}
						else
						{
							dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
							dst[dstoffset++] = putSIB(0, MODSIB64::NONE, MODSIBBASE64::NOTE);
							*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
							srcoffset += 2;
							dstoffset += 4;
						}
						if (opcode == 0xC6)
							dst[dstoffset++] = src[srcoffset++];
						else if (opcode == 0xC7)
						{
							*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
							srcoffset += 2;
							dstoffset += 2;
						}
						break;
					}
					case MODRM16::BX:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::RBX);
						break;
					}
					break;
				case 0b01:
					switch (static_cast<MODRM16>(mrmrm))
					{
					case MODRM16::BXSI:
					case MODRM16::BXDI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
						dst[dstoffset++] = putSIB(0, mrmrm | MODSIB64::RSI, MODSIBBASE64::RBX);
						dst[dstoffset++] = src[srcoffset++];
						break;
					case MODRM16::BPSI:
					case MODRM16::BPDI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
						dst[dstoffset++] = putSIB(0, MODSIB64::RBP, mrmrm | MODSIBBASE64::RSI);
						dst[dstoffset++] = src[srcoffset++];
						break;
					case MODRM16::SI:
					case MODRM16::DI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, mrmrm | MODRM64::RSI);
						dst[dstoffset++] = src[srcoffset++];
						break;
					case MODRM16::BP:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::RBP);
						dst[dstoffset++] = src[srcoffset++];
						break;
					case MODRM16::BX:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::RBX);
						dst[dstoffset++] = src[srcoffset++];
						break;
					}
					break;
				case 0b10:
					switch (static_cast<MODRM16>(mrmrm))
					{
					case MODRM16::BXSI:
					case MODRM16::BXDI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
						dst[dstoffset++] = putSIB(0, mrmrm | MODSIB64::RSI, MODSIBBASE64::RBX);
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 4;
						break;
					case MODRM16::BPSI:
					case MODRM16::BPDI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::SIB);
						dst[dstoffset++] = putSIB(0, MODSIB64::RBP, mrmrm | MODSIBBASE64::RSI);
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 4;
						break;
					case MODRM16::SI:
					case MODRM16::DI:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, mrmrm | MODRM64::RSI);
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 4;
						break;
					case MODRM16::BP:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::RBP);
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 4;
						break;
					case MODRM16::BX:
						dst[dstoffset++] = putRM(mrmmod, mrmreg, MODRM64::RBX);
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 4;
						break;
					}
					break;
				case 0b11:
				{
					dst[dstoffset++] = mrm;
					//OPCODE + IMM8
					if (src[srcoffset - 2] == 0x6B || src[srcoffset - 2] == 0x80 || src[srcoffset - 2] == 0x83 || src[srcoffset - 2] == 0xC0 ||
						src[srcoffset - 2] == 0xC1 || (src[srcoffset - 2] == 0xF6 && (mrmreg == MODREG::AL || mrmreg == MODREG::CL)))
						dst[dstoffset++] = src[srcoffset++];
					//OPCODE + IMM16
					if (src[srcoffset - 2] == 0x69 || src[srcoffset - 2] == 0x81 || (src[srcoffset - 2] == 0xF7 && (mrmreg == MODREG::AL || mrmreg == MODREG::CL)))
					{
						*reinterpret_cast<u16*>(&dst[dstoffset]) = *reinterpret_cast<u16*>(&src[srcoffset]);
						srcoffset += 2;
						dstoffset += 2;
					}
					//OPCODE + IMM32
					if (src[srcoffset - 2] == 0x81 && operandSize == 32)
					{
						*reinterpret_cast<u32*>(&dst[dstoffset]) = *reinterpret_cast<u32*>(&src[srcoffset]);
						srcoffset += 4;
						dstoffset += 4;
					}
					break;
				}
				}
				st = STATE::ENDOPCODE;
				break;
			}
			case STATE::ENDOPCODE:
				cmd16[cmdOffset] = srcoffset;
				cmd64[cmdOffset] = dstoffset;
				++cmdOffset;
				st = STATE::PREFIX;
				operandSize = 8;
				addressSize = 8;
				break;
			case STATE::ENDJUMP:
				cmdJump[cmdJumpOffset++] = cmdOffset;
				cmd16[cmdOffset] = srcoffset;
				cmd64[cmdOffset] = dstoffset;
				++cmdOffset;
				st = STATE::PREFIX;
				operandSize = 8;
				addressSize = 8;
				break;
			}
			--counter;
		}
		/*for (size_t i = 0; i < cmdJumpOffset; i++)
		{
			printf("%i %hi\n", i, cmd16[cmdJump[i] - 1]);
		}*/
		for (size_t i = 0; i < cmdJumpOffset; i++)
		{
			//получаем текущий jump который будем обрабатывать
			int cmdJumpIndex = cmdJump[i] - 1;
			//получаем смещение в scr с которого начинается текущий jump
			int cmd16JumpOffset = cmd16[cmdJumpIndex];
			int cmd16JumpOffset1 = cmd16[cmdJumpIndex + 1];
			int cmd64JumpOffset = cmd64[cmdJumpIndex];
			int cmd64JumpOffset1 = cmd64[cmdJumpIndex + 1];
			if (src[cmd16JumpOffset] == 0xEB)
			{
				++cmd16JumpOffset;
				++cmd64JumpOffset;
				i8 address16 = *reinterpret_cast<i8*>(&src[cmd16JumpOffset]);
				int findCmdIndex = search(cmd16, cmdOffset, cmd16JumpOffset1 + address16);
				if (findCmdIndex != -1)
					*reinterpret_cast<i8*>(&dst[cmd64JumpOffset]) = -(cmd64JumpOffset1 - cmd16[findCmdIndex]);
				else
					*reinterpret_cast<i8*>(&dst[cmd64JumpOffset]) -= (cmd64JumpOffset1 - cmd16JumpOffset - 1);
			}
			else if (src[cmd16JumpOffset] == 0xE8)
			{
				++cmd16JumpOffset;
				cmd64JumpOffset += 2;
				i16 address16 = *reinterpret_cast<i16*>(&src[cmd16JumpOffset]);
				int findCmdIndex = search(cmd16, cmdOffset, cmd16JumpOffset1 + address16);
				if (findCmdIndex != -1)
					*reinterpret_cast<i16*>(&dst[cmd64JumpOffset]) = -(cmd64JumpOffset1 - cmd16[findCmdIndex]);
				else
					*reinterpret_cast<i16*>(&dst[cmd64JumpOffset]) -= (cmd64JumpOffset1 - 2 - cmd16JumpOffset - 1 + 1);
			}
			else if (src[cmd16JumpOffset] == 0xE9)
			{
				++cmd16JumpOffset;
				cmd64JumpOffset += 2;
				i16 address16 = *reinterpret_cast<i16*>(&src[cmd16JumpOffset]);
				int findCmdIndex = search(cmd16, cmdOffset, cmd16JumpOffset1 + address16);
				if (findCmdIndex != -1)
					*reinterpret_cast<i16*>(&dst[cmd64JumpOffset]) = -(cmd64JumpOffset1 - cmd16[findCmdIndex]);
				else
					*reinterpret_cast<i16*>(&dst[cmd64JumpOffset]) -= (cmd64JumpOffset1 - 2 - cmd16JumpOffset - 1 + 1);
			}
			else if (src[cmd16JumpOffset + 1] >= 0x80 && src[cmd16JumpOffset + 1] <= 0x8F)
			{
				cmd16JumpOffset += 2;
				cmd64JumpOffset += 3;
				i16 address16 = *reinterpret_cast<i16*>(&src[cmd16JumpOffset]);
				int findCmdIndex = search(cmd16, cmdOffset, cmd16JumpOffset1 + address16);
				if (findCmdIndex == -1)
				{
					*reinterpret_cast<i16*>(&dst[cmd64JumpOffset]) -= (cmd64JumpOffset1 - cmd16JumpOffset) - 2;
				}
				else
					*reinterpret_cast<i16*>(&dst[cmd64JumpOffset]) = -(cmd64JumpOffset1 - cmd64[findCmdIndex]);
			}
		}
		size = dstoffset;
	}

}