﻿export module kernel;
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
import sl.algorithm;

[[noreturn]] void mainCRTStartup(const BootInfo& bootInfo)
{
	framebuffer = bootInfo.fb;
	font = bootInfo.font;
	fontSize = bootInfo.memoryMapEntries[3].sizeOfBytes;
	cpuio::cli();
	cpuio::loadGDT(&GDT::gdtDescriptor);
	console::initialize();
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	cpuio::loadIDTR(&IDT::idtr);
	cpuio::getCPUFeatures(cpuio::features);
	serial::initialize();
	memory::initialize(bootInfo);
	console::unicode = (u16*)memory::allocator::allocBlocks(memory::allocator::countBlocks(0xFFFFu * 2));
	console::unicodeInit();
	console::clear();
	console::color = console::CYAN;
	//console::putfeatures(cpuio::features);
	console::printf("%llx %llx %llx %llx\n", memory::allocator::maxBlocks, memory::allocator::reservedBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks);
	console::printf("%llx %llx %llx %llx\n", framebuffer.baseAddress, memory::allocator::memoryMap, memory::sizeRAM, memory::allocator::unusedBlocks);
	////PIT::setDivisor(PIT::divisor);
	PIT::setFrequency(20000);
	if (!(bootInfo.RSDP.isValid() && bootInfo.RSDP.XSDT.header.isValid()))
	{
		console::printf("RSDP or XSDT invalid\n");
		cpuio::loop();
	}
	ACPI::initialize(bootInfo.RSDP);
	if (ACPI::madt->flags)
		PIC::initialize();
	APIC::inittialize();
	cpuio::sti();
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