export module disk.PhysicalRAMDisk;
import disk.layout.gpt;
import driver.RAMDisk;
import types;
import memory.utils;
export namespace disk
{
	class PhysicalRAMDisk
	{
	public:
		PhysicalRAMDisk(void* addressVHD, u64 sectorCount) : driver(addressVHD, sectorCount)
		{

		}
		void loadRAMDisk()
		{
			u8* buffer = new u8[driver.getSectorSize()];
			driver.read(1, 1, buffer);
			memory::copy(&GPT, buffer, sizeof(GPT));
			delete[] buffer;
			GPTEntries = new layout::gpt::GPTPartitionEntry[GPT.numberOfPartitionEntries]{};
			int countPartitionEntryPerSector = driver.getSectorSize() / GPT.sizeOfPartitionEntry;
			int countSectorsToReadAll = GPT.numberOfPartitionEntries / countPartitionEntryPerSector;
			buffer = new u8[driver.getSectorSize() * countSectorsToReadAll];
			driver.read(GPT.partitionEntryLBA, countSectorsToReadAll, buffer);
			memory::copy(GPTEntries, buffer, GPT.numberOfPartitionEntries * sizeof(disk::layout::gpt::GPTPartitionEntry));
			u8 i = 0;
			for (; i < GPT.numberOfPartitionEntries; ++i)
			{
				if (!GPTEntries[i].isUsed())
					break;
			}
			numberOfPartitions = i + 1;
		}
		void readWithCheckLBA(const u64 lba, const u32 sectorCount, void* buffer)
		{
			if (!containsLBAInPartitions(lba))
				return;
			driver.read(lba, sectorCount, buffer);
		}
		void writeWithCheckLBA(const u64 lba, const u32 sectorCount, const void* buffer)
		{
			if (!containsLBAInPartitions(lba))
				return;
			driver.write(lba, sectorCount, buffer);
		}
		void read(const u64 lba, const u32 sectorCount, void* buffer)
		{
			driver.read(lba, sectorCount, buffer);
		}
		void write(const u64 lba, const u32 sectorCount, const void* buffer)
		{
			driver.write(lba, sectorCount, buffer);
		}
		u8 getNumberOfPartition() const
		{
			return numberOfPartitions;
		}
		u16 getSectorSize() const
		{
			return driver.getSectorSize();
		}
		const layout::gpt::GPTPartitionEntry& getGPTEntry(const u64 index) const
		{
			return GPTEntries[index];
		}
	private:
		const bool containsLBAInPartitions(const u64 lba) const
		{
			for (u8 i = 0; i < numberOfPartitions; i++)
			{
				if (lba >= GPTEntries[i].startingLBA && lba <= GPTEntries[i].endingLBA)
					return true;
			}
			return false;
		}
		layout::gpt::GPT GPT;
		layout::gpt::GPTPartitionEntry* GPTEntries;
		u8 numberOfPartitions;
		driver::RAMDisk driver;
	};
}