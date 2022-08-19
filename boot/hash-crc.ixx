export module hash:crc;
import types;
export namespace hash::crc::tables
{
	enum class Type8 : u8
	{
		DEFAULT,
		CDMA2000,
		DARC,
		DVBS2,
		EBU,
		ICODE,
		ITU,
		MAXIM,
		ROHC,
		WCDMA
	};
	u8 crc8[2560];
}
export namespace hash::crc
{
	constexpr u8 polynominal[10]{ 0x07, 0x9B, 0x39, 0xD5, 0x1D, 0x1D, 0x07, 0x31, 0x07, 0x9B };
	constexpr u8 init[10]{ 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFD, 0x00, 0x00, 0xFF, 0x00 };
	constexpr u8 xorout[10]{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00 };
	constexpr u8 width = 8 * sizeof(u8);
	constexpr u8 topbit = 1 << (width - 1);
	constexpr u64 reflect(u64 data, const u8 countBits)
	{
		u64 reflect{};
		for (u8 i = 0; i < countBits; ++i)
		{
			if (data & 0x01)
			{
				reflect |= (1ull << (countBits - 1ull - i));
			}
			data >>= 1;
		}
		return reflect;
	}
	constexpr void initialize()
	{
		u8 remainder;
		for (u16 i = 0; i < 256; ++i)
		{
			remainder = i << (width - 8);
			for (u8 j = 8; j > 0; --j)
			{
				if (remainder & topbit)
				{
					remainder = (remainder << 1) ^ polynominal[0];
				}
				else
					remainder <<= 1;
			}
			tables::crc8[i] = remainder;
		}
	}
	constexpr u8 crc(const void* data, const u64 size, const tables::Type8 type)
	{
		const u64 index = static_cast<const u64>(type);
		u8 databyte, remainder{init[index]};
		for (u64 byte = 0; byte < size; ++byte)
		{
			databyte = reinterpret_cast<const u8*>(data)[byte] ^ (remainder >> (width - 8));
			remainder = tables::crc8[databyte] ^ (remainder << 8);
		}
		return remainder ^ xorout[index];
	}
}