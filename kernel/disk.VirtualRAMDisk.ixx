export module disk.VirtualRAMDisk;
import types;
import disk.PhysicalRAMDisk;
import disk.layout.gpt;
import fs.FAT.structures;
import memory.utils;
import console;
export namespace disk
{
#pragma pack(1)
	class VirtualRAMDisk
	{

	public:
		VirtualRAMDisk(disk::PhysicalRAMDisk& physicalRAMDisk, const u64 indexEntry) : physicalRAMDisk(physicalRAMDisk), entry(physicalRAMDisk.getGPTEntry(indexEntry)) {}
		void read(const u64 lba, const u32 sectorCount, void* buffer)
		{
			if (lba + sectorCount > entry.size())
				return;
			physicalRAMDisk.read(entry.startingLBA + lba, sectorCount, buffer);
		}
		void write(const u64 lba, const u32 sectorCount, const void* buffer)
		{
			if (lba + sectorCount > entry.size())
				return;
			physicalRAMDisk.write(entry.startingLBA + lba, sectorCount, buffer);
		}
		void loadFileSystem()
		{
			u8* buffer = new u8[physicalRAMDisk.getSectorSize()];
			console::printf("%llx\n", &entry);
			physicalRAMDisk.read(entry.startingLBA, 1, buffer);
			console::puthex(&fatbs, 90);
			console::puthex(buffer, 90);
			memory::copy(&fatbs, buffer, 90);
			console::printf("%llx\n", &entry);
			console::puthex(&fatbs, 96);
			console::puthex(buffer, 96);
			i32 t = memory::cmp(&fatbs, buffer, 90);
			console::printf("\n%i", t);
			//readFAT16Entry(0);
			//console::printf("%x ", );
			/*console::printf("%x ", readFAT16Entry(1));
			console::printf("%x ", readFAT16Entry(2));
			console::printf("%x ", readFAT16Entry(3));
			console::printf("%x ", readFAT16Entry(4));*/
			delete[] buffer;
		}
	private:
		
		u16 readFAT16Entry(u32 n)
		{
			/*console::printf("%i ", fatbs.getFirstFatSector());
			u32 sector = fatbs.getFirstFatSector() + (n * 2 / fatbs.bytesPerSector);
			u32 offset = (n * 2) % fatbs.bytesPerSector;
			console::printf("%llx ", &entry);*/
			//u16 result;
			//u8* buffer = new u8[physicalRAMDisk.getSectorSize()];
			////physicalRAMDisk.read(entry.startingLBA + sector, 1, buffer);
			////result = *reinterpret_cast<u16*>(buffer + offset);
			//delete[] buffer;
			return 0;
		}
		u32 readFAT32Entry(u32 n)
		{
			const u32 sector = fatbs.getFirstFatSector() + (n * 4 / fatbs.bytesPerSector);
			const u32 offset = (n * 4) % fatbs.bytesPerSector;
			u32 result;
			u8* buffer = new u8[physicalRAMDisk.getSectorSize()];
			physicalRAMDisk.read(entry.startingLBA + sector, 1, buffer);
			result = *reinterpret_cast<u32*>(buffer + offset) & 0x0FFFFFFF;
			delete[] buffer;
			return result;
		}
		void writeFAT16Entry(u32 n, u16 value)
		{
			const u32 sector = fatbs.getFirstFatSector() + (n * 2 / fatbs.bytesPerSector);
			const u32 offset = (n * 2) % fatbs.bytesPerSector;
			u8* buffer = new u8[physicalRAMDisk.getSectorSize()];
			physicalRAMDisk.read(entry.startingLBA + sector, 1, buffer);
			*reinterpret_cast<u16*>(buffer + offset) = value;
			physicalRAMDisk.write(entry.startingLBA + sector, 1, buffer);
			delete[] buffer;
		}
		void writeFAT32Entry(u32 n, u32 value)
		{
			const u32 sector = fatbs.getFirstFatSector() + (n * 2 / fatbs.bytesPerSector);
			const u32 offset = (n * 2) % fatbs.bytesPerSector;
			u8* buffer = new u8[physicalRAMDisk.getSectorSize()];
			physicalRAMDisk.read(entry.startingLBA + sector, 1, buffer);
			u32& tmp = *reinterpret_cast<u32*>(buffer + offset);
			tmp = (tmp & 0xF0000000) | (value & 0x0FFFFFFF);
			physicalRAMDisk.write(entry.startingLBA + sector, 1, buffer);
			delete[] buffer;
		}
		disk::PhysicalRAMDisk& physicalRAMDisk;
		const disk::layout::gpt::GPTPartitionEntry& entry;
		fs::FAT::FATBS fatbs;
	};
}