export module MCFG;
import types;
import SDT;
export namespace ACPI
{
	struct MCFGHeader
	{
		SDT header;
		u64 reserved;
	};
	struct DeviceConfig
	{
		u64 baseAddress;
		u16 pciSegmentGroup;
		u8 startBus;
		u8 endBus;
		u32 reserved;
	};
}