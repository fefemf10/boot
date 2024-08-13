export module fs.FAT16.structures;
import types;
export namespace fs::FAT16
{
#pragma pack(1)
	struct FAT16EBS
	{
		u8 biosDriveNumber;
		u8 rsv0;
		u8 bootSignature;
		u32 volumeId;
		char8_t volumeLabel[11];
		char8_t fatTypeLabel[8];
	};
}