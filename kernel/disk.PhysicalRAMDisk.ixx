export module disk.PhysicalRAMDisk;
import disk.layout.gpt;
import driver.RAMDisk;
import types;
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
			driver.read(1, 1, &GPT);
			GPTEntries = new layout::gpt::GPTPartitionEntry[GPT.numberOfPartitionEntries]{};
			int countPartitionEntryPerSector = driver.getSectorSize() / GPT.sizeOfPartitionEntry;
			int countSectorsToReadAll = GPT.numberOfPartitionEntries / countPartitionEntryPerSector;
			driver.read(GPT.partitionEntryLBA, countSectorsToReadAll, GPTEntries);
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