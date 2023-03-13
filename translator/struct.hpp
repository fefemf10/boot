#pragma once
using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using f32 = float;
using f64 = double;
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