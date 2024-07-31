export module disk.RAMDisk;
import disk.layout.gpt;
import driver.RAMDisk;
export namespace disk
{
	class RAMDisk
	{
	public:
		RAMDisk(void* addressVHD, u64 sectorCount) : driver(addressVHD, sectorCount))
		{

		}
		void loadRAMDisk()
		{
			driver.read(1, 1, &GPT);
			GPTEntries = new layout::gpt::GPTPartitionEntry[GPT.numberOfPartitionEntries]{};
			int countPartitionEntryPerSector = 512 / GPT.sizeOfPartitionEntry;
			int countSectorsToReadAll = GPT.numberOfPartitionEntries / countPartitionEntryPerSector;
			driver.read(GPT.partitionEntryLBA, countSectorsToReadAll, GPTEntries);
		}
	private:
		layout::gpt::GPT GPT;
		layout::gpt::GPTPartitionEntry* GPTEntries;
		driver::RAMDisk driver;
	};
}