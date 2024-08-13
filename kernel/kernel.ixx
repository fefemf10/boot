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
import MCFG;
import PIC;
import APIC;
import ISR;
import IRQ;
import IRQ.Number;
import IDT;
import PIT;
import intrinsic;
import APICTimer;
import HPET;
import RTC;
import FADT;
import IOAPIC;
import HPETTimer;
import disk.PhysicalRAMDisk;
import sl.vector;
import sl.memory;
import sl.utility;
import disk.VirtualRAMDisk;
import vcruntime;

[[noreturn]] void mainCRTStartup(BootInfo& bootInfo)
{
	_disable();
	cpuio::loadGDT(&GDT::gdtDescriptor);
	CallConstructors();
	framebuffer = bootInfo.fb;
	font = bootInfo.font;
	fontSize = bootInfo.memoryMapEntries[3].sizeOfBytes;
	IDT::initialize();
	cpuio::loadIDTR(&IDT::idtr);
	cpuio::getCPUFeatures(cpuio::features);
	console::initialize();
	serial::initialize();
	// 0xFFFF * 2 = 512 unicode characther in font * 256 size font 16*16
	memory::initialize(bootInfo);
	console::unicode = (u16*)memory::allocator::allocBlocks(memory::allocator::countBlocks(0xFFFFu * 2));
	memory::set(console::unicode, 0, memory::allocator::countBlocks(0xFFFFu * 2) * memory::PAGESIZE);
	console::unicodeInit();
	console::clear();
	console::color = console::CYAN;
	console::printf("%llx\n", bootInfo.memoryMapEntries[1].address);
	console::printf("%llx\n", &mainCRTStartup);
	//console::putfeatures(cpuio::features);
	//console::printf("%llx %llx %llx %llx %lli MiB\n", memory::allocator::maxBlocks, memory::allocator::reservedBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks, memory::sizeRAM / 1024 / 1024);
	ACPI::initialize(bootInfo.RSDP);
	if (ACPI::madt->flags)
		PIC::deinitialize();
	APIC::initialize();
	PIT::initialize();
	PIT::setFrequency(100);
	ACPI::hpet->initialize();
	HPETTimer::frequency = 1000000000000000.0 / ACPI::hpet->getGCID().counterClkPeriod / 100000;
	ACPI::hpet->enableTimerN(ACPI::indexCurrentTimer);
	ACPI::hpet->writeTimerComparatorN(ACPI::indexCurrentTimer, ACPI::hpet->readMainTimer() + 100000);
	ACPI::hpet->writeTimerComparatorN(ACPI::indexCurrentTimer, 100000);
	ACPI::hpet->enable();
	HPETTimer::initialize();
	RTC::initialize();
	RTC::changeRate();
	RTC::read();
	IRQ::initialize();
	_enable();
	std::vector<disk::PhysicalRAMDisk> d;
	std::vector<disk::VirtualRAMDisk> v;
	d.emplace_back(bootInfo.memoryMapEntries[4].address, bootInfo.memoryMapEntries[4].sizeOfBytes).loadRAMDisk();
	v.emplace_back(d.back(), 0);
	v.back().loadFileSystem();

	//console::printf("HPET frequency: %.2f MHz tick = %f ns\n", 1000000000000000.0 / ACPI::hpet->getGCID().counterClkPeriod / 1000000, 1.0 / (1000000000000000.0 / ACPI::hpet->getGCID().counterClkPeriod) * 1000000000);
	
	/*console::printf("%llu\n", ACPI::hpet->readTimerComparatorN(ACPI::indexCurrentTimer));
	console::printf("%llu %llu\n", ACPI::currentTimerIRQLine, ACPI::indexCurrentTimer);*/
	/*RTC::read();
	u32 secondsStart = RTC::minutes * 60 + RTC::seconds;*/
	/*APICTimer::initialize();
	APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0b0);
	APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, 0xFFFFFFFF);
	HPETTimer::sleep(1);
	APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, APICTimer::DISABLE);
	u32 ticksIn10ms = 0xFFFFFFFF - APIC::lapics[0].read(APIC::LAPIC::CURRENT_COUNT);
	APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, APICTimer::PERIODIC | IRQ::Number::APICTIMER);
	APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0b0);
	APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, ticksIn10ms);
	APICTimer::frequency = HPETTimer::frequency;
	console::printf("%u\n", ticksIn10ms);*/
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