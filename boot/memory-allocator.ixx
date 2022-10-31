export module memory:allocator;
import memory.utils;
import types;
export namespace memory::allocator
{
	u64* memoryMap{};
	u64 maxBlocks{};
	u64 usedBlocks{};
	constexpr u64 BLOCKSIZE = 12; // shr 12 = 4096
	constexpr u64 BLOCKSPERBYTE = 3; // shr 3 = 8
	void setBlock(u64 bit)
	{
		memoryMap[bit >> 6] |= 1ull << (bit % 64);
	}
	void unsetBlock(u64 bit)
	{
		memoryMap[bit >> 6] &= ~(1ull << (bit % 64));
	}
	bool testBlock(u64 bit)
	{
		return memoryMap[bit >> 6] & (1ull << (bit % 64));
	}
	i64 findFree(u64 countBlocks)
	{
		if (!countBlocks)
		{
			return -1;
		}
		for (u64 i = 0; i < maxBlocks >> 6; ++i)
		{
			if (memoryMap[i] != 0xFFFFFFFF'FFFFFFFF)
			{
				for (u8 j = 0; j < 64; ++j)
				{
					const u64 bit = 1ull << j;
					if (!(memoryMap[i] & bit))
					{
						const u64 startBit = (i << 6) + j;
						u64 freeBlocks{};
						for (u64 k = 0; k < countBlocks; ++k)
						{
							freeBlocks += !testBlock(startBit + k);
							if (freeBlocks == countBlocks)
							{
								return (i << 6) + j;
							}
						}
					}
					
				}
			}
		}
		return -1;
	}
	void initialize(u64* address, const u64 size)
	{
		memoryMap = address;
		maxBlocks = size >> BLOCKSIZE;
		usedBlocks = maxBlocks;
		set(memoryMap, 0xFF, maxBlocks >> BLOCKSPERBYTE);
	}
	void setRegion(const u64* address, const u64 size)
	{
		i64 align = reinterpret_cast<const u64>(address) >> BLOCKSIZE;
		i64 countBlocks = size >> BLOCKSIZE;
		for (; countBlocks > 0; --countBlocks)
		{
			unsetBlock(align++);
			--usedBlocks;
		}
	}
	void unsetRegion(const u64* address, const u64 size)
	{
		i64 align = reinterpret_cast<const u64>(address) >> BLOCKSIZE;
		i64 countBlocks = size >> BLOCKSIZE;
		for (; countBlocks >= 0; --countBlocks)
		{
			setBlock(align++);
			++usedBlocks;
		}
	}
	void* allocBlocks(const u64 countBlocks)
	{
		if ((maxBlocks - usedBlocks) < countBlocks) return nullptr;
		const i64 startBlock = findFree(countBlocks);
		if (startBlock == -1) return nullptr;
		for (u64 i = 0; i < countBlocks; ++i)
		{
			setBlock(startBlock + i);
			
		}
		usedBlocks += countBlocks;
		u64 address = startBlock << BLOCKSIZE;
		return reinterpret_cast<u64*>(address);
	}
	void freeBlocks(const void* address, const u64 countBlocks)
	{
		const i64 startBlock = reinterpret_cast<u64>(address) >> BLOCKSIZE;
		for (u64 i = 0; i < countBlocks; ++i)
		{
			unsetBlock(startBlock + i);
		}
		usedBlocks -= countBlocks;
	}
}