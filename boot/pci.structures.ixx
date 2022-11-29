export module pci.structures;
import types;
export namespace pci
{
	struct CMD
	{
		u16 iose : 1;
		u16 mse : 1;
		u16 bme : 1;
		u16 sce : 1;
		u16 mwie : 1;
		u16 vga : 1;
		u16 pee : 1;
		u16 wcc : 1;
		u16 see : 1;
		u16 fbe : 1;
		u16 id : 1;
		u16 rsv : 5;
	};
	struct STS
	{
		u16 rsv0 : 3;
		u16 is : 1;
		u16 cl : 1;
		u16 c66 : 1;
		u16 rsv1 : 1;
		u16 fbc : 1;
		u16 dpd : 1;
		u16 devt : 1;
		u16 sta : 1;
		u16 rta : 1;
		u16 rma : 1;
		u16 sse : 1;
		u16 dpe : 1;
	};
#pragma pack(1)
	struct Header
	{
		u16 vendor;
		u16 device;
		CMD cmd;
		STS status;
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