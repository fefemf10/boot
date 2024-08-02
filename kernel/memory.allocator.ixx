export module memory.allocator;
import types;
import memory.utils;
import memory.descriptor;
export namespace memory
{
	constinit const u64 PAGESIZE = 0x1000;
	u64 sizeRAM{};
	const u64 getMemorySize(const Descriptor* map, u64 mapEntries, u64 descriptorSize)
	{
		u64 memorySizeBytes{};
		for (size_t i = 0; i < mapEntries; i++)
		{
			const Descriptor* descriptor = (Descriptor*)((u64)map + (i * descriptorSize));
			memorySizeBytes += descriptor->numberOfPages * PAGESIZE;
		}
		return memorySizeBytes;
	}
}
export namespace memory::allocator
{
	u64* memoryMap;
	u64 maxBlocks;
	u64 reservedBlocks;
	u64 usedBlocks;
	u64 unusedBlocks;
	constinit const u64 BLOCKSIZE = 12; // shr 12 = 4096
	constinit const u64 BLOCKSPERBYTE = 3; // shr 3 = 8
	constexpr void setBlock(u64 bit)
	{
		memoryMap[bit >> 6] |= 1ull << (bit & 63);
	}
	constexpr void unsetBlock(u64 bit)
	{
		memoryMap[bit >> 6] &= ~(1ull << (bit & 63));
	}
	constexpr bool testBlock(u64 bit)
	{
		return memoryMap[bit >> 6] & (1ull << (bit & 63));
	}
	constexpr u64 countBlocks(u64 size)
	{
		return (size >> BLOCKSIZE) + ((size & ((1ull << BLOCKSIZE) - 1ull)) > 0);
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
	void initializeBitmap(void* address, const u64 size)
	{
		memoryMap = reinterpret_cast<u64*>(address);
		maxBlocks = size >> BLOCKSIZE;
		unusedBlocks = maxBlocks;
		usedBlocks = 0;
		reservedBlocks = 0;
		set(memoryMap, 0x00, maxBlocks >> BLOCKSPERBYTE);
	}
	void unsetRegion(const void* address, const u64 countBlocks)
	{
		const u64 align = reinterpret_cast<const u64>(address) >> BLOCKSIZE;
		u64 countUnsets = 0;
		for (u64 i = 0; i < countBlocks; i++)
		{
			countUnsets += testBlock(align + i);
			unsetBlock(align + i);
		}
		usedBlocks -= countUnsets;
		unusedBlocks += countUnsets;
	}
	void setRegion(const void* address, const u64 countBlocks)
	{
		const u64 align = reinterpret_cast<const u64>(address) >> BLOCKSIZE;
		u64 countSets = 0;
		for (u64 i = 0; i < countBlocks; i++)
		{
			countSets += !testBlock(align + i);
			setBlock(align + i);
		}
		usedBlocks += countSets;
		unusedBlocks -= countSets;
	}
	void* allocBlocks(const u64 countBlocks)
	{
		if (unusedBlocks < countBlocks) return nullptr;
		const i64 startBlock = findFree(countBlocks);
		if (startBlock == -1) return nullptr;
		for (u64 i = 0; i < countBlocks; ++i)
			setBlock(startBlock + i);
		usedBlocks += countBlocks;
		unusedBlocks -= countBlocks;
		u64 address = startBlock << BLOCKSIZE;
		return reinterpret_cast<u64*>(address);
	}
	void freeBlocks(const void* address, const u64 countBlocks)
	{
		const u64 startBlock = reinterpret_cast<u64>(address) >> BLOCKSIZE;
		for (u64 i = 0; i < countBlocks; ++i)
			unsetBlock(startBlock + i);
		usedBlocks -= countBlocks;
		unusedBlocks += countBlocks;
	}
	void reserveBlocks(const void* address, const u64 countBlocks)
	{
		const u64 startBlock = reinterpret_cast<u64>(address) >> BLOCKSIZE;
		u64 countSets{};
		for (u64 i = 0; i < countBlocks; ++i)
		{
			countSets += !testBlock(startBlock + i);
			setBlock(startBlock + i);
		}
		reservedBlocks += countSets;
		unusedBlocks -= countSets;
	}
	void unreserveBlocks(const void* address, const u64 countBlocks)
	{
		const u64 startBlock = reinterpret_cast<u64>(address) >> BLOCKSIZE;
		u64 countUnsets = 0;
		for (u64 i = 0; i < countBlocks; ++i)
		{
			countUnsets += testBlock(startBlock + i);
			unsetBlock(startBlock + i);
		}
		reservedBlocks -= countUnsets;
		unusedBlocks += countUnsets;
	}
	void initialize(Descriptor* map, u64 mapEntries, u64 descriptorSize)
	{
		sizeRAM = getMemorySize(map, mapEntries, descriptorSize);
		void* largestFreeMemorySegment{};
		u64 largestFreeMemorySegmentSize{ memory::allocator::countBlocks(sizeRAM >> BLOCKSIZE >> BLOCKSPERBYTE) };
		
		for (size_t i = 0; i < mapEntries; i++)
		{
			const Descriptor* descriptor = (Descriptor*)((u64)map + (i * descriptorSize));
			if (descriptor->type == 7)
			{
				if (descriptor->numberOfPages >= largestFreeMemorySegmentSize)
				{
					largestFreeMemorySegment = descriptor->physicalAddress;
					largestFreeMemorySegmentSize = descriptor->numberOfPages;
				}
			}
		}
		
		initializeBitmap(largestFreeMemorySegment, sizeRAM);
		setRegion(memoryMap, memory::allocator::countBlocks(maxBlocks >> BLOCKSPERBYTE));
	}
}