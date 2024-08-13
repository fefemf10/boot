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
}