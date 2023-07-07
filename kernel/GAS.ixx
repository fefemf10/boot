export module GAS;
import types;
export namespace ACPI
{
	struct GAS
	{
		u8 AddressSpace;
		u8 BitWidth;
		u8 BitOffset;
		u8 AccessSize;
		u64 Address;
	};
}