export module ACPI;
import types;
import RSDP;
import XSDT;
import MADT;
import MCFG;
import FADT;
import HPET;
import SDT;
import RTC;
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
					if (nameEqual("APIC", header->signature))
						madt = reinterpret_cast<MADT*>(header);
					if (nameEqual("MCFG", header->signature))
						mcfg = reinterpret_cast<MCFG*>(header);
					if (nameEqual("FACP", header->signature))
						fadt = reinterpret_cast<FADT*>(header);
					if(nameEqual("HPET", header->signature))
						hpet = reinterpret_cast<HPET*>(header);
					if (fadt)
						RTC::centuryRegister = fadt->centry;
				}
			}
		}
	}
}