export module XSDT;
import SDT;
import FADT;
export namespace ACPI
{
#pragma pack(1)
	struct XSDT
	{
		SDT header;
		SDT* entries[1];
		const u64 sizeEntries() const
		{
			return (header.length - sizeof(SDT)) / 8;
		}
	};
}