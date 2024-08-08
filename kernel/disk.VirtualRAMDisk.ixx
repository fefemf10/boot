export module disk.VirtualRAMDisk;
import types;
import disk.PhysicalRAMDisk;
import disk.layout.gpt;
import console;
export namespace disk
{
	class VirtualRAMDisk
	{

	public:
		VirtualRAMDisk(disk::PhysicalRAMDisk& physicalRAMDisk) : physicalRAMDisk(physicalRAMDisk) {}
		void read(const u64 lba, const u32 sectorCount, void* buffer)
		{
			const disk::layout::gpt::GPTPartitionEntry& entry = physicalRAMDisk.getGPTEntry(0);
			if (lba + sectorCount > entry.size())
				return;
			physicalRAMDisk.read(entry.startingLBA + lba, sectorCount, buffer);
		}
		void write(const u64 lba, const u32 sectorCount, const void* buffer)
		{
			const disk::layout::gpt::GPTPartitionEntry& entry = physicalRAMDisk.getGPTEntry(0);
			if (lba + sectorCount > entry.size())
				return;
			physicalRAMDisk.write(entry.startingLBA + lba, sectorCount, buffer);
		}
	private:
		disk::PhysicalRAMDisk& physicalRAMDisk;
	};
}