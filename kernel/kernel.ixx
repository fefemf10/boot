export module kernel;
import types;
import cpuio;
import Framebuffer;
import Font;
import console;
import serial;
import memory.descriptor;
import BootInfo;
import memory;
import memory.allocator;
import memory.Heap;
import memory.PageIndex;
import memory.PageTableManager;
import memory.utils;
import sl.math;
import GDT;
import IDT;
import ISR;
import PIC;
import IRQ;
//import pci;
//import memory;
//import memory.allocator;
//import sl;
//import ACPI;
//import PIT;
//import VESA;
//import driver.AHCI.structures;
//import glm;
//import glm.vec2;
//import glm.vec3;
//import glm.vec4;
//import glm.mat2x2;
//import glm.mat4x4;
//import glm.transform;
//import sl.math;
//import sl.numbers;
//import sl.vector;
//import SMBIOS;
//import memory.utils;
//import translator;

//const char8_t* EFI_MEMORY_TYPE_STRINGS[]{
//
//	u8"EfiReservedMemoryType",
//	u8"EfiLoaderCode",
//	u8"EfiLoaderData",
//	u8"EfiBootServicesCode",
//	u8"EfiBootServicesData",
//	u8"EfiRuntimeServicesCode",
//	u8"EfiRuntimeServicesData",
//	u8"EfiConventionalMemory",
//	u8"EfiUnusableMemory",
//	u8"EfiACPIReclaimMemory",
//	u8"EfiACPIMemoryNVS",
//	u8"EfiMemoryMappedIO",
//	u8"EfiMemoryMappedIOPortSpace",
//	u8"EfiPalCode",
//};

[[noreturn]] void mainCRTStartup(BootInfo& bootInfo)
{
	framebuffer = bootInfo.fb;
	font = bootInfo.font;

	console::initialize();
	memory::initialize(bootInfo);
	framebuffer.clear(console::BLACK);

	cpuio::loadGDT(&GDT::gdtDescriptor);
	PIC::initialize();
	ISR::initialize();
	IRQ::initialize();
	cpuio::loadIDTR(&IDT::idtr);
	console::color = console::CYAN;
	//console::printf(u8"%llx %llx %llx\n", memory::allocator::maxBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks);
	//*(u8*)(0x0) = 55;
	//console::printf(u8"%llx %llx %u %u %u\n", framebuffer->baseAddress, framebuffer->bufferSize, framebuffer->width, framebuffer->height, framebuffer->pixelsPerScanline);
	//console::printf(u8"%llx %llx %llx\n", &bootInfo, memory::allocator::allocBlocks(1), memory::allocator::allocBlocks(1));
	////console::printf(u8"%llx %llx %llx %llx\n", memory::sizeRAM, memory::allocator::maxBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks);
	////console::printf(u8"%llx %llx %u %u %u\n", framebuffer->baseAddress, framebuffer->bufferSize, framebuffer->width, framebuffer->height, framebuffer->pixelsPerScanline);
	//u64* a = new u64(5);
	//u64* b = new u64(6);
	//if (a!= nullptr)
	//{
	//console::printf(u8"%llx %llx\n", a, b);
	//
	//}
	/*delete a;
	a = new u64(8);
	console::printf(u8"%llx %llx %llx\n", *a, a, b);
	delete b;
	a = new u64(10);
	console::printf(u8"%llx %llx %llx\n", *a, a, *b);*/
	cpuio::loop();
}