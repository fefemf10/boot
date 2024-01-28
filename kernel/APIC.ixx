export module APIC;
import types;
import LAPIC;
import IOAPIC;
import MADT;

export namespace APIC
{
	void* lapic;
	void* ioapic;
	APIC::LAPIC lapics[32];
	APIC::IOAPIC ioapics[10];
	u8 numlapic = 0;
	u8 numioapic = 0;
	bool isDiscrete = true;
	void initialize()
	{
		numlapic = 0;
		numioapic = 0;
		//current lapic
		APIC::lapic = reinterpret_cast<void*>(ACPI::madt->lapic);
		//find ioapic and local apic override
		for (size_t i = 0; i < ACPI::madt->sizeEntries(); i++)
		{
			if (ACPI::madt->entries[i].type == ACPI::Type::IOAPIC)
			{
				ACPI::IOAPIC* ioapic = reinterpret_cast<ACPI::IOAPIC*>(&ACPI::madt->entries[i]);
				if (ioapic->isValid())
				{
					APIC::ioapics[numioapic++].initialize(reinterpret_cast<void*>(ioapic->address), ioapic->GSIBase);
				}
			}
			if (ACPI::madt->entries[i].type == ACPI::Type::LAPICO)
			{
				ACPI::LAPICO* lapico = reinterpret_cast<ACPI::LAPICO*>(&ACPI::madt->entries[i]);
				if (lapico->isValid())
				{
					APIC::lapic = lapico->lapic;
				}
			}
		}
		//init currect logical core by index numlapic 0
		APIC::lapics[numlapic++].initialize(APIC::lapic);

		isDiscrete = !(APIC::lapics[0].version & 0x10);
		//find plalic == find logical cores on cpu
		for (size_t i = 0; i < ACPI::madt->sizeEntries(); i++)
		{
			if (ACPI::madt->entries[i].type == ACPI::Type::PLAPIC)
			{
				ACPI::PLAPIC* plapic = reinterpret_cast<ACPI::PLAPIC*>(&ACPI::madt->entries[i]);
				if (plapic->isValid() && plapic->isOnline() && APIC::lapics[0].id != plapic->apicId)
				{
					APIC::lapics[numlapic++].initialize(plapic->apicId, plapic->processorId);
				}
			}
		}
		//mask all interrupt
		for (size_t i = 0; i < numioapic; i++)
		{
			for (size_t j = 0; j < APIC::ioapics[i].redirectionSize; j++)
			{
				APIC::IOAPIC::REDTBLEntry entry = APIC::ioapics[i].readREDTBL(j);
				entry.mask = 1;
				APIC::ioapics[i].writeREDTBL(j, entry);
			}
		}
		APIC::lapics[0].write(APIC::LAPIC::SPURIOUS_INTERRUPT_VECTOR, 0xFF | 0x100); // 0xFF spurious interrupt 0x100 enable APIC
		APIC::lapics[0].write(APIC::LAPIC::TASK_PRIORITY, 0); // lapic will ignore interrupts below this level 0
	}
	extern volatile u8 aprunning = 0;  // count how many APs have started
	extern u8 bspdone = 0;      // BSP id and spinlock flag
	extern void* ap_stack = nullptr;
	extern u64 ap_stackNumberPagesPerCore = 0;
	void ap_main(const u8 apicId);
	extern void longMode();
}