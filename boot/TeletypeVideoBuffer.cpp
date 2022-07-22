#include "TeletypeVideoBuffer.hpp"
#include <cstdarg>
#include "IO.hpp"
namespace TeletypeVideoBuffer
{	
	short currentPos{};
	unsigned short positionFromCoords(unsigned char x, unsigned char y)
	{
		return y * TeletypeVideoBuffer::width + x;
	}
	void setCursorPosition(short position)
	{
		currentPos = position;
		if (currentPos < 0)
			currentPos = 0;
		else if (currentPos >= 2000)
			currentPos = 2000;
		outb(0x0F, 0x3D4);
		outb(static_cast<unsigned char>(currentPos & 0xFF), 0x3D5);
		outb(0x0E, 0x3D4);
		outb(static_cast<unsigned char>((currentPos >> 8) & 0xFF), 0x3D5);
	}
	void clear(u64 color)
	{
		const u64 value{ color << 8 | color << 24 | color << 40 | color << 56 };
		u64* video_buffer = reinterpret_cast<u64*>(videoBufferAddress);
		for (u64* i = video_buffer; i < reinterpret_cast<u64*>(videoBufferAddress + 4000u); i++)
		{
			*i = value;
		}
	}
	void puts(const char* string, unsigned char color)
	{
		short index = currentPos;
		while (*string)
		{
			switch (*string)
			{
			case 10:
				index += width;
				break;
			case 13:
				index -= index % width;
				break;
			default:
				*reinterpret_cast<unsigned char*>(videoBufferAddress + index * 2) = *string;
				*reinterpret_cast<unsigned char*>(videoBufferAddress + index * 2 + 1) = color;
				++index;
				break;
			}
			++string;
		}
		setCursorPosition(index);
	}
	void putc(char c, unsigned char color)
	{
		*reinterpret_cast<unsigned char*>(videoBufferAddress + currentPos * 2) = c;
		*reinterpret_cast<unsigned char*>(videoBufferAddress + currentPos * 2 + 1) = color;
		setCursorPosition(currentPos + 1);
	}
	unsigned int strlen(const char* str)
	{
		unsigned int size{};
		while (*str)
			++size;
		return size;
	}
	void printf_unsigned(unsigned long long number, int radix)
	{
		char buffer[32];
		int pos = 0;

		// convert number to ASCII
		do
		{
			unsigned long long rem = number % radix;
			number /= radix;
			buffer[pos++] = hexChars[rem];
		} while (number > 0);

		// print number in reverse order
		while (--pos >= 0)
			putc(buffer[pos]);
	}

	void printf_signed(long long number, int radix)
	{
		if (number < 0)
		{
			putc('-');
			printf_unsigned(-number, radix);
		}
		else printf_unsigned(number, radix);
	}
	void printf(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		State state = State::STATE_NORMAL;
		State length = State::LENGTH_DEFAULT;
		int radix = 10;
		bool sign{};
		bool number{};
		while (*fmt)
		{
			switch (state)
			{
			case State::STATE_NORMAL:
				switch (*fmt)
				{
				case '%':
					state = State::STATE_LENGTH;
					break;
				default:
					putc(*fmt);
					break;
				}
				break;
			case State::STATE_LENGTH:
				switch (*fmt)
				{
				case 'h':
					length = State::LENGTH_SHORT;
					state = State::STATE_LENGTH_SHORT;
					break;
				case 'l':
					length = State::LENGTH_INT;
					state = State::STATE_LENGTH_INT;
					break;
				default:
					goto State_STATE_SPEC;
					break;
				}
				break;
			case State::STATE_LENGTH_SHORT:
				if (*fmt == 'h')
				{
					length = State::LENGTH_SHORT_SHORT;
					state = State::STATE_SPEC;
				}
				else goto State_STATE_SPEC;
				break;
			case State::STATE_LENGTH_INT:
				if (*fmt == 'l')
				{
					length = State::LENGTH_LONG_LONG;
					state = State::STATE_SPEC;
				}
				else goto State_STATE_SPEC;
				break;
			case State::STATE_SPEC:
			State_STATE_SPEC:
				switch (*fmt)
				{
				case 'c':
					putc((char)va_arg(args, i32));
					break;
				case 's':
					puts(va_arg(args, const i8*));
					break;
				case '%':
					putc('%');
					break;
				case 'i':
				case 'd':
					radix = 10;
					sign = true;
					number = true;
					break;
				case 'u':
					radix = 10;
					sign = false;
					number = true;
					break;
				case 'X':
				case 'x':
				case 'p':
					radix = 16;
					sign = false;
					number = true;
					break;
				case 'o':
					radix = 8;
					sign = false;
					number = true;
					break;
				default: break;
				}
				if (number)
				{
					if (sign)
					{
						switch (length)
						{
						case State::LENGTH_SHORT_SHORT:
						case State::LENGTH_SHORT:
						case State::LENGTH_DEFAULT:
						case State::LENGTH_INT:
							i32 a;
							printf_signed(va_arg(args, i32), radix);
							break;
						case State::LENGTH_LONG_LONG:
							i64 b;
							printf_signed(va_arg(args, i64), radix);
						}
					}
					else
					{
						switch (length)
						{
						case State::LENGTH_SHORT_SHORT:
						case State::LENGTH_SHORT:
						case State::LENGTH_DEFAULT:
						case State::LENGTH_INT:
							u32 a;
							printf_unsigned(va_arg(args, u32), radix);
							break;
						case State::LENGTH_LONG_LONG:
							u64 b;
							printf_unsigned(va_arg(args, u64), radix);
						}
					}
				}
				state = State::STATE_NORMAL;
				length = State::LENGTH_DEFAULT;
				radix = 10;
				sign = false;
				number = false;
				break;
			}
			++fmt;
		}
		va_end(args);
	}
}