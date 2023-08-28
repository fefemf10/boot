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
		APIC::lapic = reinterpret_cast<void*>(ACPI::madt->lapic);
		for (size_t i = 0; i < ACPI::madt->sizeEntries(); i++)
		{
			if (ACPI::madt->entries[i].type == ACPI::Type::IOAPIC)
			{
				ACPI::IOAPIC* ioapic = reinterpret_cast<ACPI::IOAPIC*>(&ACPI::madt->entries[i]);
				if (ioapic->length == 12)
				{
					APIC::ioapics[numioapic++].initialize(reinterpret_cast<void*>(ioapic->address), ioapic->GSIBase);
				}
			}
			if (ACPI::madt->entries[i].type == ACPI::Type::LAPICO)
			{
				ACPI::LAPICO* lapico = reinterpret_cast<ACPI::LAPICO*>(&ACPI::madt->entries[i]);
				if (lapico->length == 12)
				{
					APIC::lapic = lapico->lapic;
				}
			}
		}
		APIC::lapics[numlapic++].initialize(APIC::lapic);
		isDiscrete = !(APIC::lapics[0].version & 0x10);
		for (size_t i = 0; i < ACPI::madt->sizeEntries(); i++)
		{
			if (ACPI::madt->entries[i].type == ACPI::Type::PLAPIC)
			{
				ACPI::PLAPIC* plapic = reinterpret_cast<ACPI::PLAPIC*>(&ACPI::madt->entries[i]);
				if (plapic->length == 8 && plapic->flags == 1 && APIC::lapics[0].id != plapic->apicId)
				{
					APIC::lapics[numlapic++].initialize(plapic->apicId, plapic->processorId);
				}
			}
		}
		for (size_t i = 0; i < numioapic; i++)
		{
			for (size_t j = 0; j < APIC::ioapics[i].redirectionSize; j++)
			{
				APIC::IOAPIC::REDTBLEntry entry = APIC::ioapics[i].readREDTBL(j);
				entry.mask = 1;
				APIC::ioapics[i].writeREDTBL(j, entry);
			}
		}
		APIC::lapics[0].write(APIC::LAPIC::SPURIOUS_INTERRUPT_VECTOR, 0x1FF);
		APIC::lapics[0].write(APIC::LAPIC::TASK_PRIORITY, 0);
		APIC::IOAPIC::REDTBLEntry RTCInterrupt{};
		RTCInterrupt.vector = 0x28;
		RTCInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		RTCInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		RTCInterrupt.pinPolarity = 0;
		RTCInterrupt.destination = APIC::lapics[0].id;
		RTCInterrupt.mask = 0;
		APIC::ioapics[0].writeREDTBL(8, RTCInterrupt);
		APIC::IOAPIC::REDTBLEntry PITInterrupt{};
		PITInterrupt.vector = 0x20;
		PITInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		PITInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		PITInterrupt.pinPolarity = 0;
		PITInterrupt.destination = APIC::lapics[0].id;
		PITInterrupt.mask = 0;
		APIC::ioapics[0].writeREDTBL(2, PITInterrupt);
		APIC::IOAPIC::REDTBLEntry keyboardInterrupt{};
		keyboardInterrupt.vector = 0x21;
		keyboardInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		keyboardInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		keyboardInterrupt.pinPolarity = 0;
		keyboardInterrupt.destination = APIC::lapics[0].id;
		keyboardInterrupt.mask = 0;
		APIC::ioapics[0].writeREDTBL(1, keyboardInterrupt);
	}
	extern volatile u8 aprunning = 0;  // count how many APs have started
	extern u8 bspdone = 0;      // BSP id and spinlock flag
	extern void* ap_stack = nullptr;
	extern u64 ap_stackNumberPagesPerCore = 0;
	void ap_main(const u8 apicId);
	extern void longMode();
}