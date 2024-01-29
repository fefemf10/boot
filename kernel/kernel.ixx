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
import IRQ.Number;
import IDT;
import PIT;
import intrinsic;
import sl.math;
import sl.memory;
import sl.atomic;
import sl.spinlock;
import APICTimer;
import sl.string_view;
import sl.print;
import HPET;
import RTC;
import FADT;
import IOAPIC;
import HPETTimer;
[[noreturn]] void mainCRTStartup(BootInfo& bootInfo)
{
	framebuffer = bootInfo.fb;
	font = bootInfo.font;
	fontSize = bootInfo.memoryMapEntries[3].sizeOfBytes;
	_disable();
	cpuio::loadGDT(&GDT::gdtDescriptor);
	IDT::initialize();
	cpuio::loadIDTR(&IDT::idtr);
	cpuio::getCPUFeatures(cpuio::features);
	console::initialize();
	serial::initialize();
	memory::initialize(bootInfo);
	// 0xFFFF * 2 = 512 unicode characther in font * 256 size font 16*16
	console::unicode = (u16*)memory::allocator::allocBlocks(memory::allocator::countBlocks(0xFFFFu * 2));
	memory::set(console::unicode, 0, memory::allocator::countBlocks(0xFFFFu * 2) * memory::PAGESIZE);
	console::unicodeInit();
	console::clear();
	console::color = console::CYAN;
	console::putfeatures(cpuio::features);
	console::printf("%llx %llx %llx %llx %lli MiB\n", memory::allocator::maxBlocks, memory::allocator::reservedBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks, memory::sizeRAM / 1024 / 1024);
	PIT::initialize();
	PIT::setFrequency(1000);
	if (!(bootInfo.RSDP.isValid() && bootInfo.RSDP.XSDT.header.isValid()))
	{
		console::printf("RSDP or XSDT invalid\n");
		while (true) _mm_pause();
	}
	ACPI::initialize(bootInfo.RSDP);
	if (ACPI::madt->flags)
		PIC::deinitialize();
	APIC::initialize();
	IRQ::initialize();
	_enable();
	/*APIC::IOAPIC::REDTBLEntry PITInterrupt = APIC::ioapics[0].readREDTBL(2);
	PITInterrupt.mask = 1;
	APIC::ioapics[0].writeREDTBL(2, PITInterrupt);*/
	RTC::initialize();
	RTC::changeRate();
	RTC::enable();
	RTC::read();
	
	ACPI::hpet->initialize();
	APIC::IOAPIC::REDTBLEntry hpetInterrupt{};
	hpetInterrupt.vector = IRQ::Number::HPET;
	hpetInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
	hpetInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
	hpetInterrupt.pinPolarity = 0;
	hpetInterrupt.destination = APIC::lapics[0].id;
	hpetInterrupt.mask = 0;
	APIC::ioapics[0].writeREDTBL(ACPI::currentTimerIRQLine, hpetInterrupt);
	console::printf("%u %u\n", ACPI::currentTimerIRQLine, ACPI::indexCurrentTimer);
	HPETTimer::initialize();
	HPETTimer::frequency = 1000000000000000.0 / ACPI::hpet->getGCID().counterClkPeriod / 100000;
	console::printf("HPET frequency: %.2f MHz tick = %f ns\n", 1000000000000000.0 / ACPI::hpet->getGCID().counterClkPeriod / 1000000, 1.0 / (1000000000000000.0 / ACPI::hpet->getGCID().counterClkPeriod) * 1000000000);
	ACPI::hpet->writeTimerComparatorN(ACPI::indexCurrentTimer, ACPI::hpet->readMainTimer() + 100000);
	ACPI::hpet->writeTimerComparatorN(ACPI::indexCurrentTimer, 100000);
	ACPI::hpet->enableTimerN(ACPI::indexCurrentTimer);
	ACPI::hpet->enable();
	/*console::printf("%llu\n", ACPI::hpet->readTimerComparatorN(ACPI::indexCurrentTimer));
	console::printf("%llu %llu\n", ACPI::currentTimerIRQLine, ACPI::indexCurrentTimer);*/
	/*RTC::read();
	u32 secondsStart = RTC::minutes * 60 + RTC::seconds;*/
	APICTimer::initialize();
	APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0b0);
	APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, 0xFFFFFFFF);
	HPETTimer::sleep(1);
	APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, APICTimer::DISABLE);
	u32 ticksIn10ms = 0xFFFFFFFF - APIC::lapics[0].read(APIC::LAPIC::CURRENT_COUNT);
	APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, APICTimer::PERIODIC | IRQ::Number::APICTIMER);
	APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0b0);
	APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, ticksIn10ms);
	APICTimer::frequency = HPETTimer::frequency;
	console::printf("%u\n", ticksIn10ms);
	while (true)
	{
		/*RTC::read();
		u32 secondsCurrent = RTC::minutes * 60 + RTC::seconds;*/
		console::printf("RTC: %llu %f\n", RTC::ticks, RTC::timeSinceBoot);
		console::printf("PIT: %llu %f\n", PIT::ticks, PIT::timeSinceBoot);
		console::printf("APIC: %llu %f\n", APICTimer::ticks, APICTimer::timeSinceBoot);
		console::printf("HPET: %llu %f\n", HPETTimer::ticks, HPETTimer::timeSinceBoot);
		console::printf("HPETMain: %llu %f\n", ACPI::hpet->readMainTimer(), ACPI::hpet->readMainTimer() / 1000000000000000.0 * ACPI::hpet->getGCID().counterClkPeriod);
		HPETTimer::sleep(1);
		console::clearBox(0, 21, 60, 5);
	}
	//APIC::BSPInitialize(bootInfo);
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

	while (true) _mm_pause();
}