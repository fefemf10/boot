export module teletype;
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
	u32 videoBufferAddress = 0xB8000;
	i16 currentPos = 0;
	u8 currentColor = B_BLACK | F_WHITE;
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
	void clear()
	{
		const u64 value{ static_cast<u64>(currentColor << 8 | currentColor << 24 | currentColor << 40 | currentColor << 56) };
		u64* video_buffer = reinterpret_cast<u64*>(videoBufferAddress);
		for (u64* i = video_buffer; i < reinterpret_cast<u64*>(videoBufferAddress + 4000u); i++)
		{
			*i = value;
		}
		setCursorPosition(0);
	}
	void puts(const char8_t* string)
	{
		short index = currentPos;
		while (*string)
		{
			switch (*string)
			{
			case 10:
				index += width;
				index -= index % width;
				break;
			default:
				*reinterpret_cast<char8_t*>(videoBufferAddress + index * 2) = *string;
				*reinterpret_cast<u8*>(videoBufferAddress + index * 2 + 1) = currentColor;
				++index;
				break;
			}
			++string;
		}
		setCursorPosition(index);
	}
	void putc(char8_t c)
	{
		*reinterpret_cast<char8_t*>(videoBufferAddress + currentPos * 2) = c;
		*reinterpret_cast<u8*>(videoBufferAddress + currentPos * 2 + 1) = currentColor;
		setCursorPosition(currentPos + 1);
	}
}