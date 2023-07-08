export module kernel;
import types;
import cpuio;
import Framebuffer;
import Font;
import console;
import memory.descriptor;
import BootInfo;
import memory;
import memory.allocator;
import memory.Heap;
import memory.PageIndex;
import memory.PageTableManager;
import memory.utils;
import GDT;
import RSDP;
import ACPI;
import serial;
import XSDT;
import MADT;
import PIC;
import APIC;
import ISR;
import IRQ;
import IDT;

[[noreturn]] void mainCRTStartup(const BootInfo& bootInfo)
{
	framebuffer = bootInfo.fb;
	font = bootInfo.font;
	cpuio::loadGDT(&GDT::gdtDescriptor);
	console::initialize();
	serial::initialize();
	memory::initialize(bootInfo);
	console::clear();
	console::color = console::CYAN;
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	cpuio::loadIDTR(&IDT::idtr);
	/*uint32_t edx = 0;
	uint32_t eax = (apic & 0xfffff100) | IA32_APIC_BASE_MSR_ENABLE;
	cpuSetMSR(IA32_APIC_BASE_MSR, eax, edx);*/
	//cpuio::getCPUFeatures(cpuio::features);
	//console::printf(u8"%lli\n", sizeof(cpuio::features));
	//console::putfeatures(cpuio::features);
	if (bootInfo.RSDP.isValid() && bootInfo.RSDP.XSDT.header.isValid())
	{
		console::printf(u8"%llx\n", &bootInfo.RSDP);
		console::printf(u8"%llx\n", &bootInfo.RSDP.XSDT);
		ACPI::initialize(bootInfo.RSDP);
		console::printf(u8"MADT size: %llx\n", ACPI::madt->sizeEntries());
		console::printf(u8"MADT flags: %x\n", ACPI::madt->flags);
		if (ACPI::madt->flags)
			PIC::initialize();
		u8 numlapic = 0;
		u8 numioapic = 0;
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
		for (size_t i = 0; i < ACPI::madt->sizeEntries(); i++)
		{
			if (ACPI::madt->entries[i].type == ACPI::Type::PLAPIC)
			{
				ACPI::PLAPIC* plapic = reinterpret_cast<ACPI::PLAPIC*>(&ACPI::madt->entries[i]);
				if (plapic->length == 8 && plapic->flags == 1)
				{
					APIC::lapics[numlapic++].initialize(APIC::lapic);
				}
			}
		}
		for (size_t i = 0; i < numioapic; i++)
		{
			console::printf(u8"%llx %llx %i %i %x %i\n", APIC::ioapics[i].physicalAddress, APIC::ioapics[i].virtualAddress, APIC::ioapics[i].id, APIC::ioapics[i].version, APIC::ioapics[i].GSIBase, APIC::ioapics[i].redirectionSize);
			for (size_t j = 0; j < APIC::ioapics[i].redirectionSize; j++)
			{
				APIC::IOAPIC::RedirectionEntry entry = APIC::ioapics[i].getRedirectionEntry(j);
				console::printf(u8"%llx %llx %llx %llx %llx %llx %llx\n", entry.vector, entry.mask, entry.deliveryMode, entry.deliveryStatus, entry.destinationMode, entry.destination, entry.pinPolarity);
			}
			console::puts(u8"\n");
		}
		console::printf(u8"Cores: %i LAPIC: %llx IOAPIC: %llx\n", numlapic, APIC::lapic, APIC::ioapics[0].physicalAddress);
		console::printf(u8"INT VEC: %x\n", APIC::lapics[0].read(APIC::LAPIC::SPURIOUS_INTERRUPT_VECTOR));
		APIC::IOAPIC::RedirectionEntry keyboardInterrupt{};
		keyboardInterrupt.vector = 0x21;
		keyboardInterrupt.deliveryMode = (u64)APIC::IOAPIC::DeliveryMode::EDGE;
		keyboardInterrupt.destinationMode = (u64)APIC::IOAPIC::DesctinationMode::PHYSICAL;
		keyboardInterrupt.pinPolarity = 0;
		keyboardInterrupt.destination = 0;
		keyboardInterrupt.mask = 0;
		APIC::ioapics[0].setRedirectionEntry(2, keyboardInterrupt);
		for (size_t i = 0; i < numioapic; i++)
		{
			console::printf(u8"%llx %llx %i %i %x %i\n", APIC::ioapics[i].physicalAddress, APIC::ioapics[i].virtualAddress, APIC::ioapics[i].id, APIC::ioapics[i].version, APIC::ioapics[i].GSIBase, APIC::ioapics[i].redirectionSize);
			for (size_t j = 0; j < APIC::ioapics[i].redirectionSize; j++)
			{
				APIC::IOAPIC::RedirectionEntry entry = APIC::ioapics[i].getRedirectionEntry(j);
				console::printf(u8"%llx %llx %llx %llx %llx %llx %llx\n", entry.vector, entry.mask, entry.deliveryMode, entry.deliveryStatus, entry.destinationMode, entry.destination, entry.pinPolarity);
			}
			console::puts(u8"\n");
		}
	}
	
	//u64* a = new u64(5);
	//u64* b = new u64(6);
	//if (a!= nullptr)
	//{
	//console::printf(u8"%llx %llx\n", a, b);
	//
	//}
	//delete a;
	//a = new u64(8);
	//console::printf(u8"%llx %llx %llx\n", *a, a, b);
	//delete b;
	//a = new u64(10);
	//console::printf(u8"%llx %llx %llx\n", *a, a, *b);
	cpuio::loop();
}