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
[[noreturn]] void mainCRTStartup(BootInfo& bootInfo)
{
	framebuffer = bootInfo.fb;
	font = bootInfo.font;
	fontSize = bootInfo.memoryMapEntries[3].sizeOfBytes;
	_disable();
	cpuio::loadGDT(&GDT::gdtDescriptor);
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	cpuio::loadIDTR(&IDT::idtr);
	cpuio::getCPUFeatures(cpuio::features);
	int cpuid[4];
	__cpuid(cpuid, 0x00);
	console::initialize();
	serial::initialize();
	memory::initialize(bootInfo);
	console::unicode = (u16*)memory::allocator::allocBlocks(memory::allocator::countBlocks(0xFFFFu * 2));
	memory::set(console::unicode, 0, memory::allocator::countBlocks(0xFFFFu * 2) * 0x1000);
	console::unicodeInit();
	console::clear();
	console::color = console::CYAN;
	console::printf("%i %04c%04c%04c\n", cpuid[0], &cpuid[1], &cpuid[3], &cpuid[2]);
	console::putfeatures(cpuio::features);
	console::printf("%llx %llx %llx %llx %lli MiB\n", memory::allocator::maxBlocks, memory::allocator::reservedBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks, memory::sizeRAM / 1024 / 1024);
	PIT::initialize();
	PIT::setFrequency(50);
	if (!(bootInfo.RSDP.isValid() && bootInfo.RSDP.XSDT.header.isValid()))
	{
		console::printf("RSDP or XSDT invalid\n");
		while (true) _mm_pause();
	}
	ACPI::initialize(bootInfo.RSDP);
	if (ACPI::madt->flags)
		PIC::deinitialize();
	APIC::initialize();
	_enable();
	if (cpuid[0] >= 0x15)
	{
		int cpuid0[4];
		__cpuid(cpuid0, 0x15);
		console::printf("%i %i %i\n", cpuid0[0] & 0xFFFF, cpuid0[1] & 0xFFFF, cpuid0[2] & 0xFFFF);
	}
	APICTimer::initialize();
	APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0b11);
	APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, 0xFFFFFFFF);
	PIT::sleep(10);
	APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, APICTimer::DISABLE);
	u32 ticksIn10ms = 0xFFFFFFFF - APIC::lapics[0].read(APIC::LAPIC::CURRENT_COUNT);
	APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, APICTimer::PERIODIC | 0x22);
	APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0b11);
	APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, 10000);
	APICTimer::frequency = 10000;
	
	console::printf("%u\n", 10000);
	console::printf("%f\n", PIT::getFrequency());
	console::printf("%f\n", APICTimer::frequency);
	RTC::initialize();
	RTC::read();
	RTC::enable();
	console::printf("%u:%u:%u %u.%u.%u\n", RTC::hours, RTC::minutes, RTC::seconds, RTC::day, RTC::month, RTC::year);
	//APICTimer::ticks = 0;
	//APICTimer::timeSinceBoot = 0.0;
	//PIT::ticks = 0;
	//PIT::timeSinceBoot = 0.0;
	//console::printf("%llu %llu %f %f\n", APICTimer::ticks, PIT::ticks, APICTimer::timeSinceBoot, PIT::timeSinceBoot);
	console::printf("%llu %llu %f %f\n", APICTimer::ticks, PIT::ticks, APICTimer::timeSinceBoot, PIT::timeSinceBoot);
	//console::setCursorPosition(0, console::currentPos.y);
	_disable();
	ACPI::hpet->initialize();
	console::printf("%llu", ACPI::hpet->read(ACPI::HPET::Registers::GCID) >> 32);
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
	
	while(true) _mm_pause();
}