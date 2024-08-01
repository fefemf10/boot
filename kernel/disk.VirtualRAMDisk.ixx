export module disk.VirtualRAMDisk;
import types;
import disk.PhysicalRAMDisk;
#include <vector>
export namespace disk
{
	class VirtualRAMDisk
	{

	public:
		VirtualRAMDisk(disk::PhysicalRAMDisk& driver) : physicalRAMDisk(physicalRAMDisk) {}
		void read(const u64 lba, const u32 sectorCount, void* buffer)
		{
			physicalRAMDisk.read(lba, sectorCount, buffer);
		}
		void write(const u64 lba, const u32 sectorCount, const void* buffer)
		{
			physicalRAMDisk.write(lba, sectorCount, buffer);
		}
	private:
		disk::PhysicalRAMDisk& physicalRAMDisk;
	};
}