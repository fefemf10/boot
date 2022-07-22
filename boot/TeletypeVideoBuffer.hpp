#pragma once
#include "stdtype.hpp"
enum Color : u8
{
	F_BLACK = 0x00,
	F_BLUE = 0x01,
	F_GREEN = 0x02,
	F_CYAN = 0x03,
	F_RED = 0x04,
	F_MAGENTA = 0x05,
	F_BROWN = 0x06,
	F_LIGHTGRAY = 0x07,
	F_DARKGRAY = 0x08,
	F_LIGHTBLUE = 0x09,
	F_LIGHTGREEN = 0x0A,
	F_LIGHTCYAN = 0x0B,
	F_LIGHTRED = 0x0C,
	F_LIGHTMAGENTA = 0x0D,
	F_YELLOW = 0x0E,
	F_WHITE = 0x0F,
	B_BLACK = 0x00,
	B_BLUE = 0x10,
	B_GREEN = 0x20,
	B_CYAN = 0x30,
	B_RED = 0x40,
	B_MAGENTA = 0x50,
	B_BROWN = 0x60,
	B_LIGHTGRAY = 0x70,
	B_DARKGRAY = 0x80,
	B_LIGHTBLUE = 0x60,
	B_LIGHTGREEN = 0xA0,
	B_LIGHTCYAN = 0xB0,
	B_LIGHTRED = 0xC0,
	B_LIGHTMAGENTA = 0xD0,
	B_YELLOW = 0xE0,
	B_WHITE = 0xF0,
};

namespace TeletypeVideoBuffer
{
	constexpr u8 width = 80;
	constexpr u8 height = 50;
	constexpr u32 videoBufferAddress = 0xB8000;
	extern i16 currentPos;
	const u8 hexChars[] = {"0123456789ABCDEF"};
	enum class State
	{
		STATE_NORMAL,
		STATE_LENGTH,
		STATE_LENGTH_SHORT,
		STATE_LENGTH_INT,
		STATE_SPEC,
		LENGTH_DEFAULT = 0,
		LENGTH_SHORT_SHORT,
		LENGTH_SHORT,
		LENGTH_INT,
		LENGTH_LONG_LONG,
	};
	static char hexToStringOutput[128]{};
	u16 positionFromCoords(u8 x, u8 y);
	void setCursorPosition(short position);
	void clear(u64 color = B_BLACK | F_WHITE);
	void puts(const char* string, u8 color = B_BLACK | F_WHITE);
	void putc(char c, u8 color = B_BLACK | F_WHITE);
	u32 strlen(const char* str);
	void printf(const char* fmt, ...);
	int* printf_number(int* argp, State length, bool sign, int radix);
	template<typename T>
	const char* hexToString(T value)
	{
		T* valuePtr = &value;
		u8* ptr;
		u8 temp;
		u8 size = (sizeof(T)) * 2 - 1;
		u8 i;
		for (i = 0; i < size; ++i)
		{
			ptr = reinterpret_cast<u8*>(valuePtr) + i;
			temp = (*ptr & 0xF0) >> 4;
			hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
			temp = (*ptr & 0x0F);
			hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
		}
		hexToStringOutput[size + 1] = 0;
		return hexToStringOutput;
	}
}