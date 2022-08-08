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
	void setOut(OUT outt)
	{
		out = outt;
	}
	void putc(u8 c)
	{
		if (out == OUT::SERIAL)
			serial::write(c);
		else
			teletype::putc(c);
	}
	void puts(const i8* str)
	{
		if (out == OUT::SERIAL)
		{
			i8* strs = const_cast<i8*>(str);
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
		const char hexChars[] = "0123456789ABCDEF";
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
	void printf(const i8* fmt, ...)
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
					puts("\n");
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
	template <typename T>
	void puth(T value)
	{
		for (size_t i = 1; i <= sizeof(T) / 2; i++)
		{
			printf((i % 8 == 0) ? "%04hx" : "%04hx ", reinterpret_cast<u16*>(&value)[i - 1]);
			if (i % 8 == 0)
			{
				puts("\n");
			}
		}
	}
	void putregs(const cpuio::regs& regs)
	{
		printf("RAX: %08llx\n", regs.rax);
		printf("RBX: %08llx\n", regs.rbx);
		printf("RCX: %08llx\n", regs.rcx);
		printf("RDX: %08llx\n", regs.rdx);
		printf("RSI: %08llx\n", regs.rsi);
		printf("RDI: %08llx\n", regs.rdi);
		printf("RSP: %08llx\n", regs.rsp);
		printf("RBP: %08llx\n", regs.rbp);
		printf("R8:  %08llx\n", regs.r8);
		printf("R9:  %08llx\n", regs.r9);
		printf("R10: %08llx\n", regs.r10);
		printf("R11: %08llx\n", regs.r11);
		printf("R12: %08llx\n", regs.r12);
		printf("R13: %08llx\n", regs.r13);
		printf("R14: %08llx\n", regs.r14);
		printf("R15: %08llx\n", regs.r15);
		printf("INC: %08llx\n", regs.interruptCode);
		printf("ERC: %08llx\n", regs.errorCode);
	}
}