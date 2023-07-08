export module ACPI;
import types;
import RSDP;
import XSDT;
import MADT;
import MCFG;
import SDT;
import memory.allocator;
import memory.PageTableManager;
import console;
import serial;
bool nameEqual(const char8_t* str, const char8_t signature[4])
{
	return *reinterpret_cast<const u32*>(str) == *reinterpret_cast<const u32*>(signature);
}
export namespace ACPI
{
	void initialize(const RSDP& RSDP)
	{
		if (RSDP.isValid() && RSDP.revision == 2)
		{
			if (RSDP.XSDT.header.isValid())
			{
				for (size_t i = 0; i < RSDP.XSDT.sizeEntries(); i++)
				{
					ACPI::SDT* header = RSDP.XSDT.entries[i];
					console::printf(u8"%llx %04c %i\n", header, header->signature, header->revision);
					if (nameEqual(u8"APIC", header->signature))
						madt = reinterpret_cast<MADT*>(header);
					if (nameEqual(u8"MCFG", header->signature))
						mcfg = reinterpret_cast<MCFG*>(header);
				}
			}
		}
	}
}