export module pci.structures;
import types;
export namespace pci
{
#pragma pack(1)
	struct Header
	{
		u16 vendor;
		u16 device;
		u16 cmd;
		u16 status;
		u8 revision;
		u8 programmingInterface;
		u8 subclassCode;
		u8 classCode;
		u8 cacheLineSize;
		u8 masterLatencyTimer;
		u8 headerType;
		u8 builtInSeltTest;
	};
#pragma pack(1)
	struct Header0
	{
		Header header;
		u32 bars[5];
		u32 bars5;
		u32 reversed;
		u16 subsystemVendor;
		u16 subsystemID;
		u32 expansionROM;
		u8 capabilityPointer;
		u32 reserved2;
		u16 reserved3;
		u8 reserved4;
		u8 interruptLine;
		u8 interruptPin;
		u8 minGrant;
		u8 maxLatency;
	};
}