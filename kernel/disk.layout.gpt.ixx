export module disk.layout.gpt;
import types;
export namespace disk::layout::gpt
{
#pragma pack(1)
	struct GPTPartitionEntry
	{
		u64 partitionTypeGUID[2];
		u64 uniquePartitionGUID[2];
		u64 startingLBA;
		u64 endingLBA;
		struct
		{
			u8 requiredPartition : 1;
			u8 noBlockIOProtocol : 1;
			u8 legacyBIOSBootable : 1;
			u8 rsv0;
			u8 rsv1;
			u8 rsv2;
			u8 rsv3;
			u8 rsv4;
			u8 rsv5;
			u8 rsv6;
		} attributes;
		char8_t partitionName[72];
		const bool isUsed() const
		{
			return partitionTypeGUID[0] || partitionTypeGUID[1];
		}
	};
	struct GPT
	{
		u64 signature;
		u32 revision;
		u32 headerSize;
		u32 headerCRC32;
		u32 rsv;
		u64 myLBA;
		u64 alternateLBA;
		u64 firstUsableLBA;
		u64 lastUsableLBA;
		u64 diskGUID[2];
		u64 partitionEntryLBA;
		u32 numberOfPartitionEntries;
		u32 sizeOfPartitionEntry;
		u32 partitionEntryArrayCRC32;
	};
}