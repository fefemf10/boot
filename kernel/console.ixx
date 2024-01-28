export module console;
import "stdarg.h";
import serial;
import cpuio;
import types;
import Framebuffer;
import Font;
import sl.math;
import sl.concepts;
import sl.typetraits;
import sl.print;
import sl.string_view;
import intrinsic1;

namespace console
{
	const u64 precisions[19] = {
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
	constexpr const char hexchar8_ts[] = "0123456789ABCDEF";
}

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
	u32 clearColor = Color::BLACK;
	struct Point
	{
		int x;
		int y;
	} currentPos;
	i64 width;
	i64 height;
	u16* unicode;
	i8 buffer[32]{};
	enum class OUT
	{
		FRAMEBUFFER,
		SERIAL
	} out{};
	void clamp(Point& point)
	{
		if (point.x < 0)
			point.x = 0;
		else if (point.x >= width)
			point.x = width - 1;
		if (point.y < 0)
			point.y = 0;
		else if (point.y >= height)
			point.y = height - 1;
	}
	void initialize()
	{
		currentPos.x = 0;
		currentPos.y = 0;
		out = OUT::FRAMEBUFFER;
		width = framebuffer.width / 8;
		height = framebuffer.height / font->charSize;
		unicode = nullptr;
	}
	void unicodeInit()
	{
		u16 glyph = 0;
		const u16* start = (u16*)(&font->glyphBuffer + font->charSize * (font->mode & 1 ? 512 : 256));
		const u16* end = (u16*)(&font->glyphBuffer + fontSize - 4);
		const u16 countRecords = static_cast<u16>(end - start);
		for (size_t i = 0; i < countRecords; i++)
		{
			if (start[i] == 0xFFFF)
			{
				++glyph;
				continue;
			}
			unicode[start[i]] = glyph;
		}
		/*u16 uc = (u16)((u16*)s[0]);
		if (uc == 0xFF) {
			glyph++;
			s++;
			continue;
		}
		else if (uc & 128)
		{
			if ((uc & 32) == 0)
			{
				uc = ((s[0] & 0x1F) << 6) + (s[1] & 0x3F);
				s++;
			}
			else if ((uc & 16) == 0)
			{
				uc = ((((s[0] & 0xF) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F);
				s += 2;
			}
			else if ((uc & 8) == 0)
			{
				uc = ((((((s[0] & 0x7) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F)) << 6) + (s[3] & 0x3F);
				s += 3;
			}
			else
				uc = 0;
		}*/
	}
	void clear()
	{
		framebuffer.clear(clearColor);
		currentPos.x = 0;
		currentPos.y = 0;
	}
	void setCursorPosition(Point position)
	{
		currentPos = position;
		clamp(currentPos);
	}
	void setCursorPosition(int x, int y)
	{
		currentPos.x = x;
		currentPos.y = y;
		clamp(currentPos);
	}
	void moveCursorPosition(Point position)
	{
		currentPos.x += position.x;
		currentPos.y += position.y;
		clamp(currentPos);
	}
	void moveCursorPosition(int x, int y)
	{
		currentPos.x += x;
		if (currentPos.x < 0)
		{
			currentPos.x = width - 1;
			--currentPos.y;
		}
		else if (currentPos.x >= width)
		{
			currentPos.x = 0;
			++currentPos.y;
		}
		currentPos.y += y;
		if (currentPos.y < 0)
		{
			currentPos.y = 0;
			currentPos.x = 0;
		}
		else if (currentPos.y >= height)
		{
			currentPos.y = height - 1;
			currentPos.x = width - 1;
		}
		clamp(currentPos);
	}
	void upCursorPosition()
	{
		moveCursorPosition(0, -1);
	}
	void downCursorPosition()
	{
		moveCursorPosition(0, 1);
	}
	void clearLine(u8 countChar)
	{
		currentPos.x = 0;
		framebuffer.drawRectangle(0, currentPos.y * font->charSize, countChar * 8, font->charSize, clearColor);
	}
	void clearLine(u16 y, u8 countChar)
	{
		currentPos.x = 0;
		framebuffer.drawRectangle(0, y * font->charSize, countChar * 8, font->charSize, clearColor);
	}
	void clearBox(u16 x, u16 y, u8 width, u8 height)
	{
		currentPos.x = x;
		currentPos.y = y;
		framebuffer.drawRectangle(x * 8, y * font->charSize, width * 8, height * font->charSize, clearColor);
	}
	void clearGlyph(Point position)
	{
		framebuffer.drawRectangle(position.x * 8, position.y * font->charSize, 8, font->charSize, clearColor);
	}

	void drawChar(char32_t c, Point position)
	{
		clearGlyph(position);
		u32* pixels = (u32*)framebuffer.baseAddress;
		if (font->mode & 2)
			c = unicode[c];
		char* fontPtr = reinterpret_cast<char*>(&font->glyphBuffer) + (c * font->charSize);
		for (size_t y = position.y * font->charSize; y < position.y * font->charSize + font->charSize; y++)
		{
			for (size_t x = position.x * 8; x < position.x * 8 + 8; x++)
			{
				if ((*fontPtr & (0b10000000 >> (x - position.x * 8))) > 0)
				{
					*(u32*)(pixels + x + (y * framebuffer.pixelsPerScanline)) = color;
				}
			}
			++fontPtr;
		}
	}
	void putc(char32_t c)
	{
		drawChar(c, currentPos);
		moveCursorPosition(1, 0);
	}
	constexpr size_t utf8_codepoint_size(u8 c)
	{
		if ((c & 0b10000000) == 0) return 1;
		if ((c & 0b11100000) == 0b11000000) return 2;
		if ((c & 0b11110000) == 0b11100000) return 3;
		return 4;
	}
	void print(std::string_view str)
	{
		std::_Unicode_codepoint_iterator s(str.cbegin(), str.cend());
		while (*s)
		{
			switch (*s)
			{
			case '\n':
				setCursorPosition(0, currentPos.y+1);
				break;
			default:
				putc(*s);
				break;
			}
			++s;
		}
	}
	void setOut(OUT outt)
	{
		out = outt;
	}
	
	void puts(std::string_view str)
	{
		if (out == OUT::FRAMEBUFFER)
			print(str);
		else
		{
			for (size_t i = 0; i < str.size(); i++)
			{
				serial::write(str[i]);
			}
		}
	}
	void printf_unsigned(u64 number, i32 radix, i64 width, bool skipfirst = false)
	{
		i8 pos = 0;
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


	template <class T>
	constexpr const bool isLessZero(T args);

	template <std::signed_integral T>
	constexpr const bool isLessZero(T args)
	{
		return args < 0;
	}

	template <std::signed_integral T>
	constexpr void printt(T first)
	{
		i8 pos = 0;
		constexpr u32 radix = 10;
		const bool isSignedAndSign = first < 0;
		if (isSignedAndSign)
			first = -first;

		do
		{
			const u64 rem = first % radix;
			first /= radix;
			buffer[pos++] = hexchar8_ts[rem];
		} while (first);

		if (isSignedAndSign)
		{
			buffer[pos] = '-';
			pos++;
		}
		while (--pos >= 0)
			putc(buffer[pos]);
	}

	template <std::unsigned_integral T>
	constexpr void printt(T first)
	{
		i8 pos = 0;
		constexpr u32 radix = 10;
		do
		{
			const u64 rem = first % radix;
			first /= radix;
			buffer[pos++] = hexchar8_ts[rem];
		} while (first);

		while (--pos >= 0)
			putc(buffer[pos]);
	}

	template <std::character T>
	constexpr void printt(const T* first)
	{
		puts(first);
	}

	template <std::character T, class... Args>
	constexpr void printt(const T* first, Args... args)
	{

		(printt(args), ...);
	}
	void printf(std::string_view fmt, ...)
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
		std::_Unicode_codepoint_iterator s(fmt.begin(), fmt.end());
		while (*s)
		{
			switch (state)
			{
			case State::STATE_NORMAL:
				switch (*s)
				{
				case '%':
					state = State::STATE_LENGTH;
					break;
				case '\n':
					puts("\n");
					break;
				case '\t':
					puts("    ");
					break;
				default:
					putc(*s);
					break;
				}
				break;
			case State::STATE_LENGTH:
				if (*s >= '0' && *s <= '9')
				{
					state = State::STATE_WIDTH;
				}
				else
				{
				State_WidthLabel:
					switch (*s)
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
				if (*s >= '0' && *s <= '9')
				{
					length = State::LENGTH_DEFAULT;
					state = State::STATE_LENGTH;
					precision = *s - 48;
				}
				break;
			case State::STATE_WIDTH:
				if (*s >= '0' && *s <= '9')
				{
					state = State::STATE_WIDTH;
					width = width * 10 + (*s - 48);
				}
				else
					goto State_WidthLabel;
				break;
			case State::STATE_LENGTH_SHORT:
				if (*s == 'h')
				{
					length = State::LENGTH_SHORT_SHORT;
					state = State::STATE_SPEC;
				}
				else goto State_STATE_SPEC;
				break;
			case State::STATE_LENGTH_INT:
				if (*s == 'l')
				{
					length = State::LENGTH_LONG_LONG;
					state = State::STATE_SPEC;
				}
				else goto State_STATE_SPEC;
				break;
			case State::STATE_SPEC:
			State_STATE_SPEC:
				switch (*s)
				{
				case 'c':
					if (width != 0)
					{
						const char* str = va_arg(args, const char*);
						for (size_t i = 0; i < width; i++)
							putc(str[i]);
					}
					else
						putc(static_cast<char>(va_arg(args, i32)));
					break;
				case 's':
					puts(va_arg(args, const char*));
					break;
				case '%':
					putc('%');
					break;
				case 'f':
					fp = true;
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
							if (fp)
								printf_f64(va_arg(args, f64), precisions[precision - 1]);
							else
								printf_signed(va_arg(args, i32), radix, width);
							break;
						case State::LENGTH_LONG_LONG:
							if (fp)
								printf_f64(va_arg(args, f64), precisions[precision - 1]);
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
			++s;
		}
		va_end(args);
	}
	void puthex(const void* data, u64 size)
	{
		for (size_t i = 1; i <= size; i++)
		{
			printf((i % 16 == 0) ? "%02x\n" : "%02x ", reinterpret_cast<const u8*>(data)[i - 1]);
		}
		if (size % 16 != 0)
			putc('\n');
	}
	void putregs(const cpuio::regs& regs)
	{
		printf("RAX: %016llx\tRBX: %016llx\n", regs.rax, regs.rbx);
		printf("RCX: %016llx\tRDX: %016llx\n", regs.rcx, regs.rdx);
		printf("RSI: %016llx\tRDI: %016llx\n", regs.rsi, regs.rdi);
		printf("RSP: %016llx\tRBP: %016llx\n", regs.rsp, regs.rbp);
		printf("R8:  %016llx\tR9:  %016llx\n", regs.r8, regs.r9);
		printf("R10: %016llx\tR11: %016llx\n", regs.r10, regs.r11);
		printf("R12: %016llx\tR13: %016llx\n", regs.r12, regs.r13);
		printf("R14: %016llx\tR15: %016llx\n", regs.r14, regs.r15);
		printf("INC: %016llx\tERC: %016llx\n", regs.interruptCode, regs.errorCode);
		printf("RIP: %016llx\n", regs.rip);
		printf("CS: %016llx\n", regs.cs);
		printf("SS: %016llx\n", regs.ss);
		printf("USERESP: %016llx\n", regs.useresp);
		printf("RFLAGS: %016llx\n", regs.rflags);
	}
	void putfeatures(const cpuio::Features& features)
	{
		printf("SSE3:   %llx PCLMUL: %llx DTES64: %llx MONITOR: %llx\n", features.SSE3, features.PCLMUL, features.DTES64, features.MONITOR);
		printf("DS_CPL: %llx VMX:    %llx SMX:    %llx EST:     %llx\n", features.DS_CPL, features.VMX, features.SMX, features.EST);
		printf("TM2:    %llx SSSE3:  %llx CID:    %llx SDBG:    %llx\n", features.TM2, features.SSSE3, features.CID, features.SDBG);
		printf("FMA:    %llx CX16:   %llx XTPR:   %llx PDCM:    %llx\n", features.FMA, features.CX16, features.XTPR, features.PDCM);
		printf("rsv1:   %llx PCID:   %llx DCA:    %llx SSE4_1:  %llx\n", features.rsv1, features.PCID, features.DCA, features.SSE4_1);
		printf("SSE4_2: %llx X2APIC: %llx MOVBE:  %llx POPCNT:  %llx\n", features.SSE4_2, features.X2APIC, features.MOVBE, features.POPCNT);
		printf("ETSC:   %llx AES:    %llx XSAVE:  %llx OSXSAVE: %llx\n", features.ETSC, features.AES, features.XSAVE, features.OSXSAVE);
		printf("AVX:    %llx F16C:   %llx RDRAND: %llx HYPERV:  %llx\n", features.AVX, features.F16C, features.RDRAND, features.HYPERVISOR);
		printf("FPU:    %llx VME:    %llx DE:     %llx PSE:     %llx\n", features.FPU, features.VME, features.DE, features.PSE);
		printf("DTSC:   %llx MSR:    %llx PAE:    %llx MCE:     %llx\n", features.DTSC, features.MSR, features.PAE, features.MCE);
		printf("CX8:    %llx APIC:   %llx rsv2:   %llx SEP:     %llx\n", features.CX8, features.APIC, features.rsv2, features.SEP);
		printf("MTRR:   %llx PGE:    %llx MCA:    %llx CMOV:    %llx\n", features.MTRR, features.PGE, features.MCA, features.CMOV);
		printf("PAT:    %llx PSE36:  %llx PSN:    %llx CLFLUSH: %llx\n", features.PAT, features.PSE36, features.PSN, features.CLFLUSH);
		printf("rsv3:   %llx DS:     %llx ACPI:   %llx MMX:     %llx\n", features.rsv3, features.DS, features.ACPI, features.MMX);
		printf("FXSR:   %llx SSE:    %llx SSE2:   %llx SS:      %llx\n", features.FXSR, features.SSE, features.SSE2, features.SS);
		printf("HTT:    %llx TM:     %llx IA64:   %llx PBE:     %llx\n", features.HTT, features.TM, features.IA64, features.PBE);
	}
}