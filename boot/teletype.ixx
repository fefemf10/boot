export module teletype;
import "stdarg.h";
import types;
import cpuio;
export namespace teletype
{
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
	constexpr u8 width = 80;
	constexpr u8 height = 25;
	constexpr u32 videoBufferAddress = 0xB8000;
	i16 currentPos = 0;
	const u8 hexChars[] = { "0123456789ABCDEF" };
	enum class State
	{
		STATE_NORMAL,
		STATE_LENGTH,
		STATE_LENGTH_SHORT,
		STATE_LENGTH_INT,
		STATE_WIDTH,
		STATE_SPEC,
		LENGTH_DEFAULT = 0,
		LENGTH_SHORT_SHORT,
		LENGTH_SHORT,
		LENGTH_INT,
		LENGTH_LONG_LONG,
	};
	char hexToStringOutput[128]{};
	u16 positionFromCoords(u8 x, u8 y)
	{
		return static_cast<u16>(y * width + x);
	}
	void setCursorPosition(i16 position)
	{
		currentPos = position;
		if (currentPos < 0)
			currentPos = 0;
		else if (currentPos >= 2000)
			currentPos = 2000;
		cpuio::outb(0x0F, 0x3D4);
		cpuio::outb(static_cast<u8>(currentPos & 0xFF), 0x3D5);
		cpuio::outb(0x0E, 0x3D4);
		cpuio::outb(static_cast<u8>((currentPos >> 8) & 0xFF), 0x3D5);
	}
	void clear(u64 color = B_BLACK | F_WHITE)
	{
		const u64 value{ color << 8 | color << 24 | color << 40 | color << 56 };
		u64* video_buffer = reinterpret_cast<u64*>(videoBufferAddress);
		for (u64* i = video_buffer; i < reinterpret_cast<u64*>(videoBufferAddress + 4000u); i++)
		{
			*i = value;
		}
		setCursorPosition(0);
	}
	void puts(const char* string, u8 color);
	void printf(const char* fmt, ...);
	template <typename T>
	void puth(T value)
	{
		for (size_t i = 1; i <= sizeof(T)/2; i++)
		{
			teletype::printf((i % 8 == 0) ? "%04hx" : "%04hx ", reinterpret_cast<u16*>(&value)[i - 1]);
			if (i % 8 == 0)
			{
				teletype::puts("\n\r");
			}
		}
	}
	void putregs(const cpuio::regs& regs)
	{
		teletype::printf("RAX: %08llx\n", regs.rax);
		teletype::printf("RBX: %08llx\n", regs.rbx);
		teletype::printf("RCX: %08llx\n", regs.rcx);
		teletype::printf("RDX: %08llx\n", regs.rdx);
		teletype::printf("RSI: %08llx\n", regs.rsi);
		teletype::printf("RDI: %08llx\n", regs.rdi);
		teletype::printf("RSP: %08llx\n", regs.rsp);
		teletype::printf("RBP: %08llx\n", regs.rbp);
		teletype::printf("R8:  %08llx\n", regs.r8);
		teletype::printf("R9:  %08llx\n", regs.r9);
		teletype::printf("R10: %08llx\n", regs.r10);
		teletype::printf("R11: %08llx\n", regs.r11);
		teletype::printf("R12: %08llx\n", regs.r12);
		teletype::printf("R13: %08llx\n", regs.r13);
		teletype::printf("R14: %08llx\n", regs.r14);
		teletype::printf("R15: %08llx\n", regs.r15);
		teletype::printf("INC: %08llx\n", regs.interruptCode);
		teletype::printf("ERC: %08llx\n", regs.errorCode);
	}
	void puts(const char* string, u8 color = B_BLACK | F_WHITE)
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
				*reinterpret_cast<i8*>(videoBufferAddress + index * 2) = *string;
				*reinterpret_cast<i8*>(videoBufferAddress + index * 2 + 1) = color;
				++index;
				break;
			}
			++string;
		}
		setCursorPosition(index);
	}
	void putc(u8 c, u8 color = B_BLACK | F_WHITE)
	{
		*reinterpret_cast<u8*>(videoBufferAddress + currentPos * 2) = c;
		*reinterpret_cast<u8*>(videoBufferAddress + currentPos * 2 + 1) = color;
		setCursorPosition(currentPos + 1);
	}
	u32 strlen(const char* str)
	{
		unsigned int size{};
		while (*str)
			++size;
		return size;
	}
	void printf_unsigned(u64 number, i32 radix, i8 width)
	{
		i8 buffer[32]{};
		i8 pos = 0;

		// convert number to ASCII
		do
		{
			u64 rem = number % radix;
			number /= radix;
			buffer[pos++] = hexChars[rem];
		} while (number > 0);

		// print number in reverse order
		if (width > pos)
		{
			i8 pad = width - pos;
			for (i8 i = 0; i < pad; i++)
			{
				putc('0');
			}
			goto print_number;
		}
		else
		{
			print_number:
			while (--pos >= 0)
				putc(buffer[pos]);
		}
	}
	void printf_signed(i64 number, i32 radix, i8 width)
	{
		if (number < 0)
		{
			putc('-');
			printf_unsigned(-number, radix, width);
		}
		else printf_unsigned(number, radix, width);
	}
	void printf(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		State state = State::STATE_NORMAL;
		State length = State::LENGTH_DEFAULT;
		i8 width = 0;
		i32 radix = 10;
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
				case '\n':
					puts("\n\r");
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
				case '0':
					length = State::LENGTH_DEFAULT;
					state = State::STATE_WIDTH;
					break;
				default:
					goto State_STATE_SPEC;
					break;
				}
				break;
			case State::STATE_WIDTH:
				if (*fmt >= '0' && *fmt <= '9')
				{
					length = State::LENGTH_DEFAULT;
					state = State::STATE_LENGTH;
					width = *fmt - 48;
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
					putc(static_cast<u8>(va_arg(args, i32)));
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
							printf_signed(va_arg(args, i32), radix, width);
							break;
						case State::LENGTH_LONG_LONG:
							printf_signed(va_arg(args, i64), radix, width);
							break;
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
							printf_unsigned(va_arg(args, u32), radix, width);
							break;
						case State::LENGTH_LONG_LONG:
							printf_unsigned(va_arg(args, u64), radix, width);
							break;
						}
					}
				}
				state = State::STATE_NORMAL;
				length = State::LENGTH_DEFAULT;
				width = 0;
				radix = 10;
				sign = false;
				number = false;
				break;
			}
			++fmt;
		}
		va_end(args);
	}
	
	template<typename T>
	const char* hexToString(const T& value)
	{
		const T* valuePtr = &value;
		const u8* ptr;
		u8 temp;
		u8 size = (sizeof(T)) * 2 - 1;
		u8 i;
		for (i = 0; i < size; ++i)
		{
			ptr = reinterpret_cast<const u8*>(valuePtr) + i;
			temp = (*ptr & 0xF0) >> 4;
			hexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
			temp = (*ptr & 0x0F);
			hexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
		}
		hexToStringOutput[size + 1] = 0;
		return hexToStringOutput;
	}
}