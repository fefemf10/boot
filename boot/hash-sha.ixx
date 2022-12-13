export module hash.sha;
import types;
import memory.utils;

export namespace hash::sha
{
	struct sha256_t
	{
		u64 data[8];
	};
	constexpr bool operator==(const sha256_t& lhs, const sha256_t& rhs)
	{
		for (u8 i = 0; i < 8; ++i)
			if (lhs.data[i] != rhs.data[i])
				return false;
		return true;
	}
	sha256_t sha256(const void* data, const u64 size)
	{
		const u64 pad = 512 - size % 512;
		const u64 newSize = (pad < 9 || pad == 512) ? ((size >> 9) + 2) * 512 : ((size >> 9) + 1) * 512;
		u8* d8 = new u8[newSize];
		u32* d32 = reinterpret_cast<u32*>(d8);
		u64* d64 = reinterpret_cast<u64*>(d8);
		memory::copy(data, d8, size);
		d8[size] = 1 << 7;
		*reinterpret_cast<u64*>(d8 + newSize - 8) = size << 3;
		return sha256_t();
	}
}