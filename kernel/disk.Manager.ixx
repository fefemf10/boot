export module disk.Manager;
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
			physicalRAMDisks.emplace_back(physicalRAMDisk);
			PhysicalRAMDisk& prd = physicalRAMDisks.back();
			prd.loadRAMDisk();
			/*for (size_t i = 0; i < prd.getNumberOfPartition(); i++)
			{
				virtualRAMDisks.emplace_back(VirtualRAMDisk(prd));
			}*/

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