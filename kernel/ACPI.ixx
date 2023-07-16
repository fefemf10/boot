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
bool nameEqual(const char* str, const char signature[4])
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
					console::printf("%llx %04c %i\n", header, header->signature, header->revision);
					if (nameEqual("APIC", header->signature))
						madt = reinterpret_cast<MADT*>(header);
					if (nameEqual("MCFG", header->signature))
						mcfg = reinterpret_cast<MCFG*>(header);
				}
			}
		}
	}
}