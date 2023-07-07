export module MADT;
import types;
import SDT;
export namespace ACPI
{
	struct MADT
	{
		SDT header;
		u32 lapic;
		u32 flags;
		struct
		{
			u8 type;
			u8 length;
		} entries[1];
	} *madt;
}