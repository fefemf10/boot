export module disk.VirtualRAMDisk;
import types;
import disk.PhysicalRAMDisk;
import disk.layout.gpt;
import fs.FAT.structures;
import memory.utils;
import sl.string_view;
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
			physicalRAMDisk.read(entry.startingLBA, 1, buffer);
			memory::copy(&fatbs, buffer, sizeof(fatbs));
			getFATDirectoryOfFile("a.txt");

			/*console::printf("%x ", readFAT16Entry(1));
			console::printf("%x ", readFAT16Entry(2));
			console::printf("%x ", readFAT16Entry(3));
			console::printf("%x ", readFAT16Entry(4));*/
			delete[] buffer;
		}
	private:
		void getFATDirectoryOfFile(std::string_view path)
		{
			u8* buffer = new u8[fatbs.getRootDirSizeInSectors() * physicalRAMDisk.getSectorSize()];
			physicalRAMDisk.read(entry.startingLBA + fatbs.getFirstRootDirSector(), fatbs.getRootDirSizeInSectors(), buffer);
			char16_t* nameFileUTF16;
			char* nameFileUTF8;
			for (size_t i = 0; i < fatbs.rootEntryCount; i++)
			{
				u8* entry = buffer + i * sizeof(fs::FAT::FATDirectory);
				if (entry[0] == 0)
					break;
				if (entry[0] == 0xE5)
					continue;
				if (entry[11] == 0x0F)
				{
					fs::FAT::LFN* lfn = reinterpret_cast<fs::FAT::LFN*>(entry);
					if (lfn->longNameFlag == 0x01)
						nameFileUTF16 = new char16_t[13 * lfn->ord + 1] {};
					memory::copy(nameFileUTF16 + (lfn->ord - 1) * 13, lfn->name1, sizeof(lfn->name1));
					memory::copy(nameFileUTF16 + (lfn->ord - 1) * 13 + 5, lfn->name2, sizeof(lfn->name2));
					memory::copy(nameFileUTF16 + (lfn->ord - 1) * 13 + 11, lfn->name3, sizeof(lfn->name3));
				}
				else
				{
					fs::FAT::FATDirectory* directory = reinterpret_cast<fs::FAT::FATDirectory*>(entry);
					if (nameFileUTF16)
					{
						for (size_t i = 0, j = 0; ; i++)
						{
							char* c = &nameFileUTF8[j];
							char* d = &nameFileUTF8[j];
							d = console::from_utf16(nameFileUTF16[i], c);
						}
						console::printf("%s", nameFileUTF8);
						delete[] nameFileUTF16;
					}
					else
					{
						console::printf("Name: %011c\tFileSize: %u", directory->name, directory->filesize);
					}
				}
			}
			delete[] buffer;
		}
		u32 getFirstSectorOfCluster(u32 n)
		{
			return fatbs.getFirstDataSector() + (n - 2) * fatbs.sectorsPerCluster;
		}
		u16 readFAT16Entry(u32 n)
		{
			u32 sector = fatbs.getFirstFatSector() + (n * 2 / fatbs.bytesPerSector);
			u32 offset = (n * 2) % fatbs.bytesPerSector;
			u16 result;
			u8* buffer = new u8[physicalRAMDisk.getSectorSize()];
			physicalRAMDisk.read(entry.startingLBA + sector, 1, buffer);
			result = *reinterpret_cast<u16*>(buffer + offset);
			delete[] buffer;
			return result;
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