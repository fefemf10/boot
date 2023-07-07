export module ACPI;
import types;
import RSDP;
import memory.PageTableManager;
import console;
export namespace ACPI
{
	void initialize(const RSDP& RSDP)
	{
		if (RSDP.isValid() && RSDP.revision == 2)
		{
			if (RSDP.XSDT.isValid())
			{
				console::printf(u8"%04c\n", RSDP.XSDT.header.signature);
				for (size_t i = 0; i < RSDP.XSDT.sizeEntries() - 1; i++)
				{
					ACPI::SDT* header = reinterpret_cast<ACPI::SDT*>((u64)RSDP.XSDT.entries[i] >> 32);
					memory::pageTableManager.mapMemory(header, header);
					console::printf(u8"%llx %04c\n", header, header->signature);
					//console::printf(u8"%04c\n", header->signature);
				}
			}
			
		}
	}
}