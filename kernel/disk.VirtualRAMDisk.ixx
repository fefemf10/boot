export module disk.VirtualRAMDisk;
import types;
import disk.PhysicalRAMDisk;
import disk.layout.gpt;
import fs.FAT.print;
import fs.FAT.structures;
import memory.utils;
import sl.string_view;
import sl.string;
import sl.cctype;
import sl.algorithm;
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
			console::printf("%x\n", getFATDirectoryOfFile("a.txt"));
			console::printf("%x ", readFAT16Entry(1));
			console::printf("%x ", readFAT16Entry(2));
			console::printf("%x ", readFAT16Entry(3));
			console::printf("%x ", readFAT16Entry(4));
			delete[] buffer;
		}
	private:
		u32 getFATDirectoryOfFile(std::string_view path)
		{
			u8* buffer = new u8[fatbs.getRootDirSizeInSectors() * physicalRAMDisk.getSectorSize()];
			physicalRAMDisk.read(entry.startingLBA + fatbs.getFirstRootDirSector(), fatbs.getRootDirSizeInSectors(), buffer);
			char16_t* nameFileUTF16;
			char* nameFileUTF8;
			u32 sector = 0;
			for (size_t i = 0; i < fatbs.rootEntryCount; i++)
			{
				u8* entry = buffer + i * sizeof(fs::FAT::FATDirectory);
				//end
				if (entry[0] == 0)
					break;
				//skip
				if (entry[0] == 0xE5)
					continue;
				//LongFileName
				if (entry[11] == 0x0F)
				{
					const fs::FAT::LFN& lfn = *reinterpret_cast<const fs::FAT::LFN*>(entry);
					if (lfn.longNameFlag == 0x01)
					{
						nameFileUTF16 = new char16_t[13 * lfn.ord + 1] {};
						nameFileUTF8 = new char[(13 * lfn.ord + 1) * 3] {};
					}
					memory::copy(nameFileUTF16 + (lfn.ord - 1) * 13, lfn.name1, sizeof(lfn.name1));
					memory::copy(nameFileUTF16 + (lfn.ord - 1) * 13 + 5, lfn.name2, sizeof(lfn.name2));
					memory::copy(nameFileUTF16 + (lfn.ord - 1) * 13 + 11, lfn.name3, sizeof(lfn.name3));
				}
				else
				{
					std::string filename;
					if (entry[0] == 0x05)
						entry[0] = 0xE5;
					const fs::FAT::FATDirectory& directory = *reinterpret_cast<const fs::FAT::FATDirectory*>(entry);
					if (nameFileUTF16)
					{
						char* c = nameFileUTF8;
						size_t i = 0;
						for (;; i++)
						{
							if (nameFileUTF16[i] == 0)
								break;
							c = console::from_utf16(nameFileUTF16[i], c);
						}
						filename = nameFileUTF8;
						delete[] nameFileUTF8;
						delete[] nameFileUTF16;
						nameFileUTF8 = nullptr;
						nameFileUTF16 = nullptr;
					}
					else
					{
						size_t i = 0;
						for (;; i++)
							if (directory.name[i] == ' ' || directory.name[i] == 0)
								break;
						filename.append(reinterpret_cast<const char*>(directory.name), i);
						filename.push_back('.');
						filename.append(reinterpret_cast<const char*>(directory.name+8), 3);
						sector = directory.firstClusterHI << 16 | directory.firstClusterLO;
					}
					if (directory.isFile())
					{
						console::printf("%s %s\n", filename.data(), path.data());
						std::transform(filename.begin(), filename.end(), filename.begin(), std::tolower);
						console::printf("FileName: %s\n", filename.data());
						console::printf("FileSize: %u\n", directory.filesize);
						console::printf("Create Date: "); fs::FAT::print(directory.createDate);
						console::printf("\tCreate Time: "); fs::FAT::print(directory.createTime); console::print("\n");
						console::printf("Write Date: "); fs::FAT::print(directory.writeDate);
						console::printf("\tWrite Time: "); fs::FAT::print(directory.writeTime); console::print("\n");
						if (filename == path)
						{
							delete[] buffer;
							return sector;
						}
					}
				}
			}
			delete[] buffer;
			return sector;
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