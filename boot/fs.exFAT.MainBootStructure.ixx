export module fs;
import types;
export namespace fs::exFAT
{
	struct MainBoot
	{
		u8 jumpBoot[3];
		char8_t fileSystemName[8];
		u8 mustBeZero[53];
		u64 partitionOffset;
		u64 volumeLength;
		u32 fatOffset;
		u32 fatLength;
		u32 clusterHeapOffset;
		u32 clusterCount;
		u32 firstClusterOfRootDirectory;
		u32 volumeSerialNumber;
		u16 fileSystemRevision;
		u16 volumeFlags;
		u8 bytesPerSectorShift;
		u8 sectorsPerClusterShift;
		u8 numberOfFats;
		u8 driveSelect;
		u8 percentInUse;
		u8 rsv[7];
		u8 bootCode[390];
		u8 bootSignature[2];
	};
}