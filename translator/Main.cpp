#include <iostream>
#include <fstream>
#include <vector>
#include "struct.hpp"

//#define LOOP1(a) a
//#define LOOP2(a) a LOOP1(a)
//#define LOOP3(a) a LOOP2(a)
//
//#define LOOPN(n,a) LOOP##a(case n:)

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
i64 functionMap64[100]{};
i64 functionMap16[100]{};
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
	int cmd16[1000]{};
	int cmd64[1000]{};
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
				src[srcoffset] == 0xE9 || src[srcoffset] == 0x68)
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
				CASEM(0x68, 0)
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

int main()
{
	std::ifstream f1("test1.bin", std::ios::in | std::ios::binary);
	std::vector<u8> bit16(std::istreambuf_iterator<char>(f1), {});
	f1.close();
	std::vector<u8> bit64(10240, 0);
	size_t s{};
	translator(bit16.data(), bit64.data(), s);
	std::ofstream f("serial.txt", std::ios::binary | std::ios::out);
	f.write((i8*)bit64.data(), s);
	f.close();
	return 0;
}