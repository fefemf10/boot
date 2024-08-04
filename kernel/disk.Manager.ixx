export module disk;
import disk.PhysicalRAMDisk;
import disk.VirtualRAMDisk;
import types;
import sl.vector;
export namespace disk
{
	class Manager
	{
	public:
		void addPhysicalRAMDisk(PhysicalRAMDisk physicalRAMDisk)
		{

		}
		VirtualRAMDisk& getVirtualDisk(u64 index)
		{
			return virtualRAMDisks[index];
		}
	private:
		
		std::vector<PhysicalRAMDisk> physicalRAMDisks;
		std::vector<VirtualRAMDisk> virtualRAMDisks;
	};
}