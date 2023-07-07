export module XSDT;
import SDT;
import FADT;
export namespace ACPI
{
	struct XSDT
	{
		SDT header;
		void* entries[1];
		constexpr bool isValid() const
		{
			u8 sum{};
			for (size_t i = 0; i < header.length; i++)
				sum += reinterpret_cast<u8*>(const_cast<XSDT*>(this))[i];
			return sum == 0;
		}
		constexpr u64 sizeEntries() const
		{
			return (header.length - sizeof(SDT)) / 8;
		}
	};
}