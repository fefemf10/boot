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
	struct ExtendedBoot
	{
		u8 extendedBootCode[1];
		u32 extendedBootSignature;
	};
	struct GenericParameters
	{
		u8 parametersGuid[16];
		u8 rsv[32];
	};
	struct NullParameters
	{
		u8 parametersGuid[16];
		u8 rsv[16];
	};
	struct FlashParameters
	{
		u8 parametersGuid[16];
		u32 eraseBlockSize;
		u32 pageSize;
		u32 spareSection;
		u32 randomAccessTime;
		u32 programmingTime;
		u32 readCycle;
		u32 writeCycle;
		u32 rsv;
	};
	struct OEMParameters
	{
		GenericParameters parameters[10];
	};
}