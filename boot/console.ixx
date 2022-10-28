export module console;
import "stdarg.h";
import teletype;
import serial;
import cpuio;
import types;
import math;
export namespace console
{
	enum class OUT
	{
		TELETYPE,
		SERIAL
	} out;
	void initialize()
	{
		teletype::clear();
		serial::initialize();
	}
	void setOut(OUT outt)
	{
		out = outt;
	}
	void putc(char8_t c)
	{
		if (out == OUT::SERIAL)
			serial::write(c);
		else
			teletype::putc(c);
	}
	void puts(const char8_t* str)
	{
		if (out == OUT::SERIAL)
		{
			char8_t* strs = const_cast<char8_t*>(str);
			while (*strs)
			{
				serial::write(*strs++);
			}
		}
		else
			teletype::puts(str);
	}
	void printf_unsigned(u64 number, i32 radix, i64 width)
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
		while (--pos >= 0)
			putc(buffer[pos]);
	}
	void printf_signed(i64 number, i32 radix, i64 width)
	{
		if (number < 0)
			putc('-'), -number;
		printf_unsigned(number, radix, width);
	}
	void printf_f64(double number, u64 precision)
	{
		if (number < 0)
			putc(u8'-');
		const f64 absnum = math::abs(number);
		const u64 intnum = static_cast<u64>(absnum);
		const u64 fracnum = static_cast<u64>((absnum - static_cast<f64>(intnum)) * precision);
		printf_unsigned(intnum, 10, 0);
		putc(u8'.');
		printf_unsigned(fracnum, 10, 0);
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
		i8 precision = 0;
		i32 radix = 10;
		bool sign{};
		bool number{};
		bool fp{};
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
					putc(static_cast<u8>(va_arg(args, i32)));
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
								printf_f64(va_arg(args, f64), 1000000000);
							else
								printf_signed(va_arg(args, i32), radix, width);
							break;
						case State::LENGTH_LONG_LONG:
							if (fp)
								printf_f64(va_arg(args, f64), 1000000000);
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
				sign = false;
				number = false;
				fp = false;
				break;
			}
			++fmt;
		}
		va_end(args);
	}
	void puth(const void* data, u64 size)
	{
		for (size_t i = 1; i <= size / 8; i++)
		{
			printf((i % 2 == 0) ? u8"%016x\n" : u8"%016x ", reinterpret_cast<const u64*>(data)[i - 1]);
		}
	}
	void putregs(const cpuio::regs& regs)
	{
		printf(u8"RAX: %08llx\n", regs.rax);
		printf(u8"RBX: %08llx\n", regs.rbx);
		printf(u8"RCX: %08llx\n", regs.rcx);
		printf(u8"RDX: %08llx\n", regs.rdx);
		printf(u8"RSI: %08llx\n", regs.rsi);
		printf(u8"RDI: %08llx\n", regs.rdi);
		printf(u8"RSP: %08llx\n", regs.rsp);
		printf(u8"RBP: %08llx\n", regs.rbp);
		printf(u8"R8:  %08llx\n", regs.r8);
		printf(u8"R9:  %08llx\n", regs.r9);
		printf(u8"R10: %08llx\n", regs.r10);
		printf(u8"R11: %08llx\n", regs.r11);
		printf(u8"R12: %08llx\n", regs.r12);
		printf(u8"R13: %08llx\n", regs.r13);
		printf(u8"R14: %08llx\n", regs.r14);
		printf(u8"R15: %08llx\n", regs.r15);
		printf(u8"INC: %08llx\n", regs.interruptCode);
		printf(u8"ERC: %08llx\n", regs.errorCode);
	}
}