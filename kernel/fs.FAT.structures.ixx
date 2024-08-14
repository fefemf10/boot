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
	struct FATDirectory
	{
		char8_t name[11];
		u8 attribute;
		u8 ntres;
		u8 createTimeTenth;
		u16 createTime;
		u16 createDate;
		u16 lastAccessDate;
		u16 firstClusterHI;
		u16 writeTime;
		u16 writeDate;
		u16 firstClusterLO;
		u32 filesize;
		u8 getChecksum() const
		{
			u8 sum{};
			for (int i = 0; i < 11; ++i)
				sum = (sum >> 1) + (sum << 7) + name[i];
			return sum;
		}
	};
	struct LFN
	{
		u8 ord : 6;
		u8 longNameFlag : 2;
		char8_t name1[10];
		u8 attribute;
		u8 type;
		u8 checksum;
		char8_t name2[12];
		u16 firsuClusterLO;
		char8_t name3[4];
	};
}