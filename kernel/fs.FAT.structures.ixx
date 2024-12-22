export module fs.FAT.structures;
import fs.FAT16.structures;
import fs.FAT32.structures;
import types;
export namespace fs::FAT
{
	enum class Type
	{
		ExFAT,
		FAT12,
		FAT16,
		FAT32
	};
#pragma pack(1)
	struct FATBS
	{
		u8 bootjmp[3];
		char8_t OEMName[8];
		u16 bytesPerSector;
		u8 sectorsPerCluster;
		u16 reservedSectorCount;
		u8 tableCount;
		u16 rootEntryCount;
		u16 totalSectors16;
		u8 mediaType;
		u16 tableSize16;
		u16 sectorsPerTrack;
		u16 headSideCount;
		u32 hiddenSectorCount;
		u32 totalSectors32;
		union 
		{
			fs::FAT16::FAT16EBS fat16;
			fs::FAT32::FAT32EBS fat32;
		} extendedBS;
		u32 getSectorCount() const
		{
			return totalSectors16 ? totalSectors16 : totalSectors32;
		}
		u32 getFirstFatSector() const
		{
			return reservedSectorCount;
		}
		u32 getOneFATSizeInSectors() const
		{
			return tableSize16 ? tableSize16 : extendedBS.fat32.tableSize32;
		}
		u32 getFATsSizeInSectors() const
		{
			return getOneFATSizeInSectors() * tableCount;
		}
		u32 getFirstRootDirSector() const
		{
			return getFirstFatSector() + getFATsSizeInSectors();
		}
		u32 getRootDirSizeInSectors() const
		{
			return (rootEntryCount * 32 + bytesPerSector - 1) / bytesPerSector;
		}
		u32 getFirstDataSector() const
		{
			return getFirstRootDirSector() + getRootDirSizeInSectors();
		}
		u32 getCountOfDataSectors() const
		{
			return getSectorCount() - getFirstDataSector();
		}
		u32 getCountOfClusters() const
		{
			return getCountOfDataSectors() / sectorsPerCluster;
		}
		Type getFATType() const
		{
			const u32 totalClusters = getCountOfClusters();
			if (bytesPerSector == 0)
			{
				return Type::ExFAT;
			}
			else if (totalClusters < 4085)
			{
				return Type::FAT12;
			}
			else if (totalClusters < 65525)
			{
				return Type::FAT16;
			}
			else
			{
				return Type::FAT32;
			}
		}
	};
	union FATDate
	{
		u16 date;
		struct
		{
			u16 day : 5;
			u16 month : 4;
			u16 year : 7;
		};
	};
	union FATTime
	{
		u16 time;
		struct
		{
			u16 seconds : 5;
			u16 minutes : 6;
			u16 hours : 5;
		};
	};
	struct FATDirectory
	{
		char8_t name[11];
		u8 attribute;
		u8 ntres;
		u8 createTimeTenth;
		FATTime createTime;
		FATDate createDate;
		FATDate lastAccessDate;
		u16 firstClusterHI;
		FATTime writeTime;
		FATDate writeDate;
		u16 firstClusterLO;
		u32 filesize;
		u8 getChecksum() const
		{
			u8 sum{};
			for (int i = 0; i < 11; ++i)
				sum = (sum >> 1) + (sum << 7) + name[i];
			return sum;
		}
		bool isDirectory() const
		{
			return attribute & 0x10;
		}
		bool isFile() const
		{
			return !(attribute & 0x10);
		}
		
	};
	struct LFN
	{
		u8 ord : 6;
		u8 longNameFlag : 2;
		char16_t name1[5];
		u8 attribute;
		u8 type;
		u8 checksum;
		char16_t name2[6];
		u16 firsuClusterLO;
		char16_t name3[2];
	};
}