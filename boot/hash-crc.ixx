export module hash:crc;
import types;
export namespace hash::crc::table
{
	u8 crc8[10][256];
	u16 crc16[23][256];
	u32 crc32[9][256];
}
export namespace hash::crc
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
	enum class Type16 : u8
	{
		CCITTFALSE,
		ARC,
		AUGCCITT,
		BUYPASS,
		CDMA2000,
		DDS110,
		DECTR,
		DECTX,
		DNP,
		EN13757,
		GENIBUS,
		MAXIN,
		MCRF4XX,
		RIELLO,
		T10DIF,
		TELEDISK,
		TMS37157,
		USB,
		A,
		KERMIT,
		MODBUS,
		X25,
		XMODEM
	};
	enum class Type32 : u8
	{
		DEFAULT,
		BZIP2,
		C,
		D,
		JAMCRC,
		MPEG2,
		POSIX,
		Q,
		XFER
	};
	constexpr u8 polynominal8[10]{ 0x07, 0x9B, 0x39, 0xD5, 0x1D, 0x1D, 0x07, 0x31, 0x07, 0x9B };
	constexpr u8 init8[10]{ 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFD, 0x00, 0x00, 0xFF, 0x00 };
	constexpr u8 xorout8[10]{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00 };
	constexpr u8 reflectin8[10]{ 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01 };
	constexpr u8 reflectout8[10]{ 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01 };
	constexpr u8 width8 = 8 * sizeof(u8);
	constexpr u8 topbit8 = 1 << (width8 - 1);
	constexpr u16 polynominal16[23]{ 0x1021, 0x8005, 0x1021, 0x8005, 0xC867, 0x8005, 0x0589, 0x0589, 0x3D65, 0x3D65, 0x1021, 0x8005, 0x1021, 0x1021, 0x8BB7, 0xA097, 0x1021, 0x8005, 0x1021, 0x1021, 0x8005, 0x1021, 0x1021 };
	constexpr u16 init16[23]{ 0xFFFF, 0x0000, 0x1D0F, 0x0000, 0xFFFF, 0x800D, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xB2AA, 0x0000, 0x0000, 0x89EC, 0xFFFF, 0xC6C6, 0x0000, 0xFFFF, 0xFFFF, 0x0000 };
	constexpr u16 xorout16[23]{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000 };
	constexpr u8 reflectin16[23]{ 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 };
	constexpr u8 reflectout16[23]{ 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 };
	constexpr u8 width16 = 8 * sizeof(u16);
	constexpr u16 topbit16 = 1 << (width16 - 1);
	constexpr u32 polynominal32[9]{ 0x04C11DB7, 0x04C11DB7, 0x1EDC6F41, 0xA833982B, 0x04C11DB7, 0x04C11DB7, 0x04C11DB7, 0x814141AB, 0x000000AF };
	constexpr u32 init32[9]{ 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
	constexpr u32 xorout32[9]{ 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
	constexpr u8 reflectin32[9]{ 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 };
	constexpr u8 reflectout32[9]{ 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 };
	constexpr u8 width32 = 8 * sizeof(u32);
	constexpr u32 topbit32 = 1 << (width32 - 1);
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
		{
			u8 remainder;
			for (u8 k = 0; k < 10; ++k)
			{
				for (u16 i = 0; i < 256; ++i)
				{
					remainder = i << (width8 - 8);
					for (u8 j = 8; j > 0; --j)
					{
						if (remainder & topbit8)
						{
							remainder = (remainder << 1) ^ polynominal8[k];
						}
						else
							remainder <<= 1;
					}
					table::crc8[k][i] = remainder;
				}
			}
		}
		{
			u16 remainder;
			for (u8 k = 0; k < 23; ++k)
			{
				for (u16 i = 0; i < 256; ++i)
				{
					remainder = i << (width16 - 8);
					for (u8 j = 8; j > 0; --j)
					{
						if (remainder & topbit16)
						{
							remainder = (remainder << 1) ^ polynominal16[k];
						}
						else
							remainder <<= 1;
					}
					table::crc16[k][i] = remainder;
				}
			}
		}
		{
			u32 remainder;
			for (u8 k = 0; k < 9; ++k)
			{
				for (u16 i = 0; i < 256; ++i)
				{
					remainder = i << (width32 - 8);
					for (u8 j = 8; j > 0; --j)
					{
						if (remainder & topbit32)
						{
							remainder = (remainder << 1) ^ polynominal32[k];
						}
						else
							remainder <<= 1;
					}
					table::crc32[k][i] = remainder;
				}
			}
		}
	}
	constexpr u8 crc8(const void* data, const u64 size, const Type8 type)
	{
		const u64 index = static_cast<const u64>(type);
		u8 databyte, remainder{ init8[index] };
		for (u64 byte = 0; byte < size; ++byte)
		{
			databyte = (reflectin8[index] ? reflect(reinterpret_cast<const u8*>(data)[byte], 8) : reinterpret_cast<const u8*>(data)[byte]) ^ (remainder >> (width8 - 8));
			remainder = table::crc8[index][databyte] ^ (remainder << 8);
		}
		return (reflectout8[index] ? reflect(remainder, width8) : remainder) ^ xorout8[index];
	}
	constexpr u16 crc16(const void* data, const u64 size, const Type16 type)
	{
		const u64 index = static_cast<const u64>(type);
		u16 databyte, remainder{ init16[index] };
		for (u64 byte = 0; byte < size; ++byte)
		{
			databyte = (reflectin16[index] ? reflect(reinterpret_cast<const u8*>(data)[byte], 8) : reinterpret_cast<const u8*>(data)[byte]) ^ (remainder >> (width16 - 8));
			remainder = table::crc16[index][databyte] ^ (remainder << 8);
		}
		return (reflectout16[index] ? reflect(remainder, width16) : remainder) ^ xorout16[index];
	}
	constexpr u32 crc32(const void* data, const u64 size, const Type32 type)
	{
		const u64 index = static_cast<const u64>(type);
		u32 databyte, remainder{ init32[index] };
		for (u64 byte = 0; byte < size; ++byte)
		{
			databyte = (reflectin32[index] ? reflect(reinterpret_cast<const u8*>(data)[byte], 8) : reinterpret_cast<const u8*>(data)[byte]) ^ (remainder >> (width32 - 8));
			remainder = table::crc32[index][databyte] ^ (remainder << 8);
		}
		return (reflectout32[index] ? reflect(remainder, width32) : remainder) ^ xorout32[index];
	}
}