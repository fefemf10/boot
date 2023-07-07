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


[[noreturn]] void mainCRTStartup(BootInfo& bootInfo)
{
	framebuffer = bootInfo.fb;
	font = bootInfo.font;
	console::initialize();
	memory::initialize(bootInfo);
	console::clear();
	cpuio::loadGDT(&GDT::gdtDescriptor);
	console::color = console::CYAN;
	//cpuio::getCPUFeatures(cpuio::features);
	//console::putfeatures(cpuio::features);
	memory::pageTableManager.mapMemory((void*)&bootInfo.RSDP, (void*)&bootInfo.RSDP);
	memory::pageTableManager.mapMemory((void*)&bootInfo.RSDP.XSDT, (void*)&bootInfo.RSDP.XSDT);
	console::printf(u8"%llx %llx\n", &bootInfo.RSDP.XSDT, &bootInfo.RSDP);
	console::printf(u8"%04c\n", bootInfo.RSDP.XSDT.header.signature);
	ACPI::initialize(bootInfo.RSDP);
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