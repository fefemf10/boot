export module disk.layout.mbr;
import types;
export namespace disk::layout::mbr
{
#pragma pack(1)
	struct CHS
	{
		u8 cylynder;
		u8 head;
		u8 sector;
	};
	struct PartitionRecord
	{
		u8 bootIndicator;
		CHS startingCHS;
		u8 osType;
		CHS endingCHS;
		u32 startingLBA;
		u32 sizeInLBA;
	};
	struct MBR
	{
		u8 bootCode[440];
		u32 uniqueMBRDiskSignature;
		u16 rsv1;
		PartitionRecord partitionRecord[4];
		u16 signature;
	};
}