#pragma once
enum Color : unsigned char
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
	constexpr unsigned char width = 80;
	constexpr unsigned char height = 50;
	constexpr unsigned int videoBufferAddress = 0xB8000;
	extern short currentPos;
	const char hexChars[] = {"0123456789ABCDEF"};
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
	unsigned short positionFromCoords(unsigned char x, unsigned char y);
	void setCursorPosition(short position);
	void clear(unsigned long long color = B_BLACK | F_WHITE);
	void puts(const char* string, unsigned char color = B_BLACK | F_WHITE);
	void putc(char c, unsigned char color = B_BLACK | F_WHITE);
	unsigned int strlen(const char* str);
	/*void printf(const char* fmt, ...);
	int* printf_number(int* argp, State length, bool sign, int radix);*/
	template<typename T>
	const char* hexToString(T value)
	{
		T* valuePtr = &value;
		unsigned char* ptr;
		unsigned char temp;
		unsigned char size = (sizeof(T)) * 2 - 1;
		unsigned char i;
		for (i = 0; i < size; ++i)
		{
			ptr = reinterpret_cast<unsigned char*>(valuePtr) + i;
			temp = (*ptr & 0xF0) >> 4;
			hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
			temp = (*ptr & 0x0F);
			hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
		}
		hexToStringOutput[size + 1] = 0;
		return hexToStringOutput;
	}
}