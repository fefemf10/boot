export module console;
import "stdarg.h";
import serial;
import cpuio;
import types;
import Framebuffer;
import Font;
import sl.math;
export namespace console
{
	enum Color
	{
		BLACK = 0x000C0C0C,
		BLUE = 0x000037DA,
		CYAN = 0x003A96DD,
		GREEN = 0x0013A10E,
		PURPLE = 0x00881798,
		RED = 0x00C50F1F,
		WHITE = 0x00CCCCCC,
		YELLOW = 0x00C19C00,
		BRIGHTBLACK = 0x00767676,
		BRIGHTBLUE = 0x003B78FF,
		BRIGHTCYAN = 0x0061D6D6,
		BRIGHTGREEN = 0x0016C60C,
		BRIGHTPURPLE = 0x00B4009E,
		BRIGHTRED = 0x00E74856,
		BRIGHTWHITE = 0x00F2F2F2,
		BRIGHTYELLOW = 0x00F9F1A5
	};
	u32 color = 0x00FFFFFF;
	void nextPositionCursor()
	{
		cursorPos.x += 8;
		if (cursorPos.x + 8 > framebuffer->width)
		{
			cursorPos.x = 0;
			cursorPos.y += 16;
		}
	}
	void nextLineCursor()
	{
		cursorPos.x = 0;
		cursorPos.y += 16;
	}
	void putChar(char8_t c, u32 offsetX, u32 offsetY)
	{
		u32* pixels = (u32*)framebuffer->baseAddress;
		char8_t* fontPtr = reinterpret_cast<char8_t*>(&font->glyphBuffer) + (c * font->charSize);
		for (size_t y = offsetY; y < offsetY + 16; y++)
		{
			for (size_t x = offsetX; x < offsetX + 8; x++)
			{
				if ((*fontPtr & (0b10000000 >> (x - offsetX))) > 0)
				{
					*(u32*)(pixels + x + (y * framebuffer->pixelsPerScanline)) = color;
				}
			}
			++fontPtr;
		}
		nextPositionCursor();
	}

	void print(const char8_t* str)
	{
		const char8_t* c = str;
		while (*c)
		{
			if (*c == u8'\n')
				nextLineCursor();
			else
				putChar(*c, cursorPos.x, cursorPos.y);
			++c;
		}
	}
	enum class OUT
	{
		FRAMEBUFFER,
		TELETYPE,
		SERIAL
	} out;
	void initialize()
	{
		serial::initialize();
	}
	void setOut(OUT outt)
	{
		out = outt;
	}
	void putc(char8_t c)
	{
		if (out == OUT::FRAMEBUFFER)
			putChar(c, cursorPos.x, cursorPos.y);
		else if (out == OUT::SERIAL)
			serial::write(c);
	}
	void puts(const char8_t* str)
	{
		if (out == OUT::FRAMEBUFFER)
			print(str);
		else if (out == OUT::SERIAL)
		{
			char8_t* strs = const_cast<char8_t*>(str);
			while (*strs)
			{
				serial::write(*strs++);
			}
		}
	}
	void printf_unsigned(u64 number, i32 radix, i64 width, bool skipfirst = false)
	{
		i8 buffer[32]{};
		i8 pos = 0;
		const char8_t hexchar8_ts[] = u8"0123456789ABCDEF";
		do
		{
			const u64 rem = number % radix;
			number /= radix;
			buffer[pos++] = hexchar8_ts[rem];
		} while (number);
		for (i8 i = 0; i < width - pos; i++)
			putc('0');
		pos -= skipfirst ? 1 : 0;
		while (--pos >= 0)
			putc(buffer[pos]);
	}
	void printf_signed(i64 number, i32 radix, i64 width)
	{
		if (number < 0)
			putc('-'), number = -number;
		printf_unsigned(number, radix, width);
	}
	void printf_f64(f64 number, u64 precision)
	{
		if (number < 0)
			putc(u8'-');
		const f64 absnum = std::abs(number);
		const u64 intnum = static_cast<u64>(absnum);
		const u64 fracnum = static_cast<u64>((absnum - static_cast<f64>(intnum) + 1.0) * precision);
		printf_unsigned(intnum, 10, 0);
		putc(u8'.');
		printf_unsigned(fracnum, 10, 0, true);
	}
	enum class State
	{
		STATE_NORMAL,
		STATE_LENGTH,
		STATE_LENGTH_SHORT,
		STATE_LENGTH_INT,
		STATE_WIDTH,
		STATE_PRECISION,
		STATE_SPEC,
		LENGTH_DEFAULT = 0,
		LENGTH_SHORT_SHORT,
		LENGTH_SHORT,
		LENGTH_INT,
		LENGTH_LONG_LONG
	};
	void printf(const char8_t* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		State state = State::STATE_NORMAL;
		State length = State::LENGTH_DEFAULT;
		i64 width = 0;
		i8 precision = 15;
		i32 radix = 10;
		bool sign{};
		bool number{};
		bool fp{};
		u64 precisions[19] = {
			10,
			100,
			1000,
			10000,
			100000,
			1000000,
			10000000,
			100000000,
			1000000000,
			10000000000,
			100000000000,
			1000000000000,
			10000000000000,
			100000000000000,
			1000000000000000,
			10000000000000000,
			100000000000000000,
			1000000000000000000,
			10000000000000000000,
		};
		while (*fmt)
		{
			switch (state)
			{
			case State::STATE_NORMAL:
				switch (*fmt)
				{
				case u8'%':
					state = State::STATE_LENGTH;
					break;
				case u8'\n':
					puts(u8"\n");
					break;
				case u8'\t':
					puts(u8"    ");
					break;
				default:
					putc(*fmt);
					break;
				}
				break;
			case State::STATE_LENGTH:
				if (*fmt >= u8'0' && *fmt <= u8'9')
				{
					state = State::STATE_WIDTH;
				}
				else
				{
					State_WidthLabel:
					switch (*fmt)
					{
					case u8'h':
						length = State::LENGTH_SHORT;
						state = State::STATE_LENGTH_SHORT;
						break;
					case u8'l':
						length = State::LENGTH_INT;
						state = State::STATE_LENGTH_INT;
						break;
					case u8'.':
						state = State::STATE_PRECISION;
						break;
					default:
						goto State_STATE_SPEC;
						break;
					}
				}
				break;
			case State::STATE_PRECISION:
				if (*fmt >= u8'0' && *fmt <= u8'9')
				{
					length = State::LENGTH_DEFAULT;
					state = State::STATE_LENGTH;
					precision = *fmt - 48;
				}
				break;
			case State::STATE_WIDTH:
				if (*fmt >= u8'0' && *fmt <= u8'9')
				{
					state = State::STATE_WIDTH;
					width = width * 10 + (*fmt - 48);
				}
				else
					goto State_WidthLabel;
				break;
			case State::STATE_LENGTH_SHORT:
				if (*fmt == u8'h')
				{
					length = State::LENGTH_SHORT_SHORT;
					state = State::STATE_SPEC;
				}
				else goto State_STATE_SPEC;
				break;
			case State::STATE_LENGTH_INT:
				if (*fmt == u8'l')
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
				case u8'c':
					putc(static_cast<char8_t>(va_arg(args, i32)));
					break;
				case u8's':
					puts(va_arg(args, const char8_t*));
					break;
				case u8'%':
					putc(u8'%');
					break;
				case u8'f':
					fp = true;
				case u8'i':
				case u8'd':
					radix = 10;
					sign = true;
					number = true;
					break;
				case u8'u':
					radix = 10;
					sign = false;
					number = true;
					break;
				case u8'X':
				case u8'x':
				case u8'p':
					radix = 16;
					sign = false;
					number = true;
					break;
				case u8'o':
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
							if (fp)
								printf_f64(va_arg(args, f64), precisions[precision-1]);
							else
								printf_signed(va_arg(args, i32), radix, width);
							break;
						case State::LENGTH_LONG_LONG:
							if (fp)
								printf_f64(va_arg(args, f64), precisions[precision-1]);
							else
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
				precision = 15;
				sign = false;
				number = false;
				fp = false;
				break;
			}
			++fmt;
		}
		va_end(args);
	}
	void puthex(const void* data, u64 size)
	{
		for (size_t i = 1; i <= size / 8; i++)
		{
			printf((i % 4 == 0) ? u8"%016llx\n" : u8"%016llx ", std::byteswap(reinterpret_cast<const u64*>(data)[i - 1]));
		}
	}
	void putregs(const cpuio::regs& regs)
	{
		printf(u8"RAX: %016llx\tRBX: %016llx\n", regs.rax, regs.rbx);
		printf(u8"RCX: %016llx\tRDX: %016llx\n", regs.rcx, regs.rdx);
		printf(u8"RSI: %016llx\tRDI: %016llx\n", regs.rsi, regs.rdi);
		printf(u8"RSP: %016llx\tRBP: %016llx\n", regs.rsp, regs.rbp);
		printf(u8"R8:  %016llx\tR9:  %016llx\n", regs.r8, regs.r9);
		printf(u8"R10: %016llx\tR11: %016llx\n", regs.r10, regs.r11);
		printf(u8"R12: %016llx\tR13: %016llx\n", regs.r12, regs.r13);
		printf(u8"R14: %016llx\tR15: %016llx\n", regs.r14, regs.r15);
		printf(u8"INC: %016llx\tERC: %016llx\n", regs.interruptCode, regs.errorCode);
		printf(u8"RIP: %016llx\n", regs.rip);
		printf(u8"CS: %016llx\n", regs.cs);
		printf(u8"SS: %016llx\n", regs.ss);
		printf(u8"USERESP: %016llx\n", regs.useresp);
		printf(u8"RFLAGS: %016llx\n", regs.rflags);
	}
}