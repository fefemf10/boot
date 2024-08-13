export module fs.FAT32.structures;
import types;
export namespace fs::FAT32
{
#pragma pack(1)
	struct FAT32EBS
	{
		u32 tableSize32;
		u16 extendedFlags;
		u16 fatVersion;
		u32 rootCluster;
		u16 fatInfo;
		u16 backupBSSector;
		u8 rsv0[12];
		u8 driveNumber;
		u8 rsv1;
		u8 bootSignature;
		u32 volumeId;
		char8_t volumeLabel[11];
		char8_t fatTypeLabel[8];
	};
}