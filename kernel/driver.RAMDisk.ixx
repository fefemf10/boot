export module driver.RAMDisk;
import types;
import memory.utils;
export namespace driver
{
	class RAMDisk
	{
	public:
		RAMDisk(void* memory, u64 sectorCount) : memory(memory), sectorCount(sectorCount), sectorSize(512) {}
		void read(const u64 lba, const u32 sectorCount, void* buffer)
		{
			if (lba + sectorCount > this->sectorCount || sectorCount <= 0)
				return;
			memory::copy(buffer, reinterpret_cast<u8*>(memory) + lba * sectorSize, sectorCount * sectorSize);
		}
		void write(const u64 lba, const u32 sectorCount, const void* buffer)
		{
			if (lba + sectorCount > this->sectorCount || sectorCount <= 0)
				return;
			memory::copy(reinterpret_cast<u8*>(memory) + lba * sectorSize, buffer, sectorCount * sectorSize);
		}
		const u64 getSectorCount() const
		{
			return sectorCount;
		}
		const u16 getSectorSize() const
		{
			return sectorSize;
		}
	private:
		void* memory;
		u64 sectorCount;
		u16 sectorSize;
	};
}