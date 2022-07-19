#include "TeletypeVideoBuffer.hpp"
#include "IO.hpp"
namespace TeletypeVideoBuffer
{	
	unsigned short positionFromCoords(unsigned char x, unsigned char y)
	{
		return y * TeletypeVideoBuffer::width + x;
	}
	void setCursorPosition(unsigned short positon)
	{
		outb(0x0F, 0x3D4);
		outb(static_cast<unsigned char>(positon & 0xFF), 0x3D5);
		outb(0x0E, 0x3D4);
		outb(static_cast<unsigned char>((positon >> 8) & 0xFF), 0x3D5);
		currentPos = positon;
	}
	void clear(unsigned long long color)
	{
		const unsigned long long value{ color << 8 | color << 24 | color << 40 | color << 56 };
		unsigned long long* video_buffer = reinterpret_cast<unsigned long long*>(videoBufferAddress);
		for (unsigned long long* i = video_buffer; i < reinterpret_cast<unsigned long long*>(videoBufferAddress + 4000u); i++)
		{
			*i = value;
		}
	}
	void puts(const char* string, unsigned char color)
	{
		const char* str = string;
		unsigned short index = currentPos;
		while (*string)
		{
			switch (*string)
			{
			case 10:
				index += TeletypeVideoBuffer::width;
				break;
			case 13:
				index -= index % TeletypeVideoBuffer::width;
				break;
			default:
				*reinterpret_cast<unsigned char*>(videoBufferAddress + index * 2) = *string;
				*reinterpret_cast<unsigned char*>(videoBufferAddress + index * 2 + 1) = color;
				++index;
				break;
			}
			string++;
		}
		TeletypeVideoBuffer::setCursorPosition(index);
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
	void printf(const char* fmt, ...)
	{
		int* argp = reinterpret_cast<int*>(&fmt);
		State state = State::STATE_NORMAL;
		State length = State::LENGTH_DEFAULT;
		int radix = 10;
		bool sign{};
		argp++;
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
				case 'i':
					length = State::LENGTH_INT;
					state = State::STATE_LENGTH_INT;
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
					putc(static_cast<char>(*argp));
					++argp;
					break;
				case 's':
					if (length == State::LENGTH_INT || length == State::LENGTH_LONG_LONG)
					{
						puts(*reinterpret_cast<char**>(argp));
						argp+=2;
					}
					else
					{
						puts(*reinterpret_cast<char**>(argp));
						++argp;
					}
					break;
				case '%':
					putc('%');
					break;
				case 'i':
					radix = 10;
					sign = true;
					argp = printf_number(argp, length, sign, radix);
					break;
				case 'u':
					radix = 10;
					sign = false;
					argp = printf_number(argp, length, sign, radix);
					break;
				case 'X':
				case 'x':
				case 'p':
					radix = 16;
					sign = false;
					break;
				case 'o':
					radix = 8;
					sign = false;
					break;
				default: break;
				}
				state = State::STATE_NORMAL;
				state = State::LENGTH_DEFAULT;
				radix = 10;
				sign = false;
				break;
			}
			++fmt;
		}
	}
	int* printf_number(int* argp, State length, bool sign, int radix)
	{
		char buffer[32]{};
		unsigned long long number;
		int number_sign = 1;
		int pos = 0;
		switch (length)
		{
		case TeletypeVideoBuffer::State::LENGTH_DEFAULT:
		case TeletypeVideoBuffer::State::LENGTH_SHORT_SHORT:
		case TeletypeVideoBuffer::State::LENGTH_SHORT:
			if (sign)
			{
				int n = *argp;
				if (n < 0)
				{
					n = -n;
					number_sign = -1;
				}
				number = static_cast<unsigned long long>(n);
			}
			else
			{
				number = *reinterpret_cast<unsigned int*>(argp);
			}
			++argp;
			break;
		case TeletypeVideoBuffer::State::LENGTH_INT:
			if (sign)
			{
				int n = *argp;
				if (n < 0)
				{
					n = -n;
					number_sign = -1;
				}
				number = static_cast<unsigned long long>(n);
			}
			else
			{
				number = *reinterpret_cast<unsigned int*>(argp);
			}
			argp += 2;
			break;
		case TeletypeVideoBuffer::State::LENGTH_LONG_LONG:
			if (sign)
			{
				long long n = *reinterpret_cast<long long*>(argp);
				if (n < 0)
				{
					n = -n;
					number_sign = -1;
				}
				number = static_cast<unsigned long long>(n);
			}
			else
			{
				number = *reinterpret_cast<unsigned long long*>(argp);
			}
			argp += 4;
			break;
		}
		do
		{
			unsigned int rem = number % radix;
			buffer[pos++] = hexChars[rem];
		} while (number > 0);
		if (sign && number_sign < 0)
			buffer[pos++] = '-';
		while (--pos >= 0)
			putc(buffer[pos]);
		return argp;
	}
}