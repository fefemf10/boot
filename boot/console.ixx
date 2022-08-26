export module console;
import "stdarg.h";
import teletype;
import serial;
import cpuio;
import types;
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
	void printf_unsigned(u64 number, i32 radix, i8 width)
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
	void printf_signed(i64 number, i32 radix, i8 width)
	{
		if (number < 0)
			putc('-'), -number;
		printf_unsigned(number, radix, width);
	}
	constexpr double xpow2(int e)
	{
		double result = 1.0;
		constexpr double table[] = { 2.0, 4.0, 8.0, 16.0, 32.0, 64.0 };
		constexpr i64 size = sizeof(table) / sizeof(table[0]);

		if (e > 0)
		{
			if (e > size)
			{
				result = table[size - 1];
				for (i64 i = 0; i < e - size; ++i)
					result *= 2.0;
			}
			else
				result = table[e - 1];
		}
		return result;
	}

	void printf_f32(const float number, u64 precision)
	{
		struct fp
		{
			u32 mantis : 23;
			u32 exp : 8;
			u32 sign : 1;
		};
		const fp num = *reinterpret_cast<const fp*>(&number);
		if (num.sign)
			putc(u8'-');

		constexpr double mpow[] = { 0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125, 0.00390625, 0.001953125, 0.0009765625 };
		//constexpr u64 mpow[] = { 5, 25, 125, 625, 3125, 15625, 78125, 390625, 1953125 };
		double sum = 1.0;
		for (size_t i = 0; i < 10; i++)
		{
			if (num.mantis >> (22 - i) & 0x1)
				sum += mpow[i];
		}
		printf_unsigned(static_cast<u64>(xpow2(num.exp - 127) * (sum)), 10, 0);
		putc(u8'.');
		printf_unsigned(static_cast<u64>(precision * (sum - 1.0)), 10, 0);
		putc(u8' ');
		printf_unsigned(*reinterpret_cast<const u64*>(&number), 16, 0);

	}
	void printf_f64(double number, u64 precision)
	{
		struct fp
		{
			u64 mantis : 52;
			u64 exp : 11;
			u64 sign : 1;
		};
		const fp num = *reinterpret_cast<const fp*>(&number);
		if (num.sign)
			putc(u8'-');

		constexpr double mpow[] = { 0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125, 0.00390625, 0.001953125, 0.0009765625 };
		//constexpr u64 mpow[] = { 5, 25, 125, 625, 3125, 15625, 78125, 390625, 1953125 };
		double sum = 1.0;
		for (size_t i = 0; i < 10; i++)
		{
			if (num.mantis >> (51 - i) & 0x1)
				sum += mpow[i];
		}
		printf_unsigned(static_cast<u64>(xpow2(num.exp - 1023) * (sum)), 10, 0);
		putc(u8'.');
		printf_unsigned(static_cast<u64>(precision * (sum - 1.0)), 10, 0);
		putc(u8' ');
		printf_unsigned(*reinterpret_cast<const u64*>(&number), 16, 0);
	}
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
		LENGTH_LONG_LONG
	};
	void printf(const char8_t* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		State state = State::STATE_NORMAL;
		State length = State::LENGTH_DEFAULT;
		i8 width = 0;
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
				case u8'0':
					length = State::LENGTH_DEFAULT;
					state = State::STATE_WIDTH;
					break;
				default:
					goto State_STATE_SPEC;
					break;
				}
				break;
			case State::STATE_WIDTH:
				if (*fmt >= u8'0' && *fmt <= u8'9')
				{
					length = State::LENGTH_DEFAULT;
					state = State::STATE_LENGTH;
					width = *fmt - 48;
				}
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
				if (*fmt == u8'f')
				{
					length = State::LENGTH_LONG_LONG;
					goto State_STATE_SPEC;
				}
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
								printf_f32((float)va_arg(args, f64), 1000000000);
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
	template <typename T>
	void puth(T value)
	{
		for (size_t i = 1; i <= sizeof(T) / 2; i++)
		{
			printf((i % 8 == 0) ? u8"%04hx" : u8"%04hx ", reinterpret_cast<u16*>(&value)[i - 1]);
			if (i % 8 == 0)
			{
				puts(u8"\n");
			}
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