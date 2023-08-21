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
	//cpuio::getCPUFeatures(cpuio::features);
	console::initialize();
	serial::initialize();
	memory::initialize(bootInfo);
	console::unicode = (u16*)memory::allocator::allocBlocks(memory::allocator::countBlocks(0xFFFFu * 2));
	memory::set(console::unicode, 0, memory::allocator::countBlocks(0xFFFFu * 2) * 0x1000);
	console::unicodeInit();
	console::clear();
	console::color = console::CYAN;
	//console::putfeatures(cpuio::features);
	//console::printf("%llx %llx %llx %llx %lli MiB\n", memory::allocator::maxBlocks, memory::allocator::reservedBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks, memory::sizeRAM / 1024 / 1024);
	PIT::setFrequency(10000);
	PIT::timeSinceBoot = 0.0;
	PIT::ticks = 0;
	if (!(bootInfo.RSDP.isValid() && bootInfo.RSDP.XSDT.header.isValid()))
	{
		console::printf("RSDP or XSDT invalid\n");
		while (true) _mm_pause();
	}
	ACPI::initialize(bootInfo.RSDP);
	if (ACPI::madt->flags)
		PIC::initialize();
	APIC::initialize();
	_enable();
	//int cpuid0[4];
	//int cpuid1[4];
	//__cpuid(cpuid0, 0x15);
	//__cpuid(cpuid1, 0x16);
	//console::printf("%i %i %i\n", cpuid0[0], cpuid0[1], cpuid0[2]);
	//console::printf("%i %i %i\n", cpuid1[0], cpuid1[1], cpuid1[2]);
	//APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0x3);
	//APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, 0xFFFFFFFF);
	//PIT::sleep(1000);
	//APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, 0x10000);
	//u32 ticksIn10ms = 0xFFFFFFFF - APIC::lapics[0].read(APIC::LAPIC::CURRENT_COUNT);
	//APIC::lapics[0].write(APIC::LAPIC::LVT_TIMER, 32 | 0x20022);
	//APIC::lapics[0].write(APIC::LAPIC::DIVIDE_CONFIGURATION, 0x3);
	//APIC::lapics[0].write(APIC::LAPIC::INITIAL_COUNT, ticksIn10ms);
	//APICTimer::frequency = ticksIn10ms;
	//APICTimer::ticks = 0;
	//APICTimer::timeSinceBoot = 0.0;
	//console::printf("%u\n", ticksIn10ms);
	constexpr const std::string_view s("a");
	std::_Unicode_codepoint_iterator s1(s.cbegin(), s.cend());
	++s1;
	const bool is = *s1 == '\0';
	console::printf("%i\n", is);

	while (1)
	{
		console::print("as");
		//console::printf("%llu", PIT::ticks);
		//i64 index = console::currentPos;
		//console::setCursorPosition(index - index % console::width);
	}
	//while (true)
	//{
	//	//i64 index = console::currentPos;
	//	//i64 cursorY = console::currentPos / console::width;
	//	//console::printf("%i %i %i %i %i", 0, cursorY * font->charSize, framebuffer.width, font->charSize, 0x00000000);
	//	//framebuffer.drawRectangle(0, cursorY * font->charSize, framebuffer.width, font->charSize, 0x00000000);
	//	//console::setCursorPosition(index - index % console::width);
	//	
	//	console::printf("%llu %llu ", APICTimer::frequency, ticksIn10ms);
	//	PIT::sleep(1000);
	//	console::clearLine(console::currentPos);
	//	PIT::sleep(1000);
	//}
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