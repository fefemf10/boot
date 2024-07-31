export module driver.RAMDisk;
import types;
import memory.utils;
export namespace driver
{
	class RAMDisk
	{
	public:
		RAMDisk(void* memory, u64 sectorCount) : memory(memory), sectorCount(sectorCount){}
		void read(const u64 lba, const u32 sectorCount, void* buffer)
		{
			if (lba > this->sectorCount || lba + sectorCount > this->sectorCount)
				return;
			memory::copy(buffer, reinterpret_cast<u8*>(memory) + lba * sectorSize, sectorCount * sectorSize);
		}
		void write(const u64 lba, const u32 sectorCount, void* buffer)
		{
			if (lba > this->sectorCount || lba + sectorCount > this->sectorCount)
				return;
			memory::copy(reinterpret_cast<u8*>(memory) + lba * sectorSize, buffer, sectorCount * sectorSize);
		}
	private:
		void* memory;
		u64 sectorCount;
		u16 sectorSize = 512;
	};
}