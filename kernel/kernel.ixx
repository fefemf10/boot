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
//import IDT;
//import ISR;
//import IRQ;
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

const char8_t* EFI_MEMORY_TYPE_STRINGS[]{

	u8"EfiReservedMemoryType",
	u8"EfiLoaderCode",
	u8"EfiLoaderData",
	u8"EfiBootServicesCode",
	u8"EfiBootServicesData",
	u8"EfiRuntimeServicesCode",
	u8"EfiRuntimeServicesData",
	u8"EfiConventionalMemory",
	u8"EfiUnusableMemory",
	u8"EfiACPIReclaimMemory",
	u8"EfiACPIMemoryNVS",
	u8"EfiMemoryMappedIO",
	u8"EfiMemoryMappedIOPortSpace",
	u8"EfiPalCode",
};

[[noreturn]] void mainCRTStartup(BootInfo& bootInfo)
{
	framebuffer = bootInfo.fb;
	font = bootInfo.font;
	console::color = console::CYAN;
	memory::allocator::initialize(bootInfo.map, bootInfo.mapEntries, bootInfo.descriptorSize);
	memory::allocator::setRegion(nullptr, 1);
	memory::allocator::setRegion(reinterpret_cast<const void*>(reinterpret_cast<u64>(bootInfo.kernelAddress) - bootInfo.kernelStackSize), memory::allocator::countBlocks(bootInfo.kernelStackSize));
	memory::allocator::setRegion(bootInfo.kernelAddress, memory::allocator::countBlocks(bootInfo.kernelSize));
	memory::allocator::setRegion(font, memory::allocator::countBlocks(bootInfo.kernelResourcesSize));
	if (reinterpret_cast<u64>(framebuffer->baseAddress) < memory::sizeRAM)
	{
		memory::allocator::setRegion(framebuffer->baseAddress, memory::allocator::countBlocks(framebuffer->bufferSize));
	}
	memory::set(framebuffer->baseAddress, 0x00, framebuffer->bufferSize);
	for (size_t i = 0; i < bootInfo.mapEntries; i++)
	{
		const memory::Descriptor* descriptor = (memory::Descriptor*)((u64)bootInfo.map + (i * bootInfo.descriptorSize));
		switch (descriptor->type)
		{
		case 0:
		case 5:
		case 6:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			memory::allocator::setRegion(descriptor->physicalAddress, descriptor->numberOfPages);
			console::printf(u8"%llx %llx %llx %s\n", descriptor->physicalAddress, descriptor->virtualAddress, descriptor->numberOfPages, EFI_MEMORY_TYPE_STRINGS[descriptor->type]);
			break;
		default:
			break;
		}
	}
	memory::PLM4 = reinterpret_cast<memory::PageTable*>(memory::allocator::allocBlocks(1));
	memory::set(memory::PLM4, 0, sizeof(memory::PageTable));
	memory::pageTableManager = memory::PageTableManager(memory::PLM4);
	//pageTableManager.mapMemory((void*)0, (void*)0, memory::sizeRAM);
	//memory::pageTableManager.mapMemory((void*)((u64)bootInfo.kernelAddress - bootInfo.kernelStackSize), (void*)((u64)bootInfo.kernelAddress - bootInfo.kernelStackSize), memory::allocator::countBlocks(bootInfo.kernelStackSize) * 0x1000);
	//memory::pageTableManager.mapMemory((void*)bootInfo.kernelAddress, (void*)bootInfo.kernelAddress, memory::allocator::countBlocks(bootInfo.kernelSize) * 0x1000);
	//memory::pageTableManager.mapMemory((void*)font, (void*)font, memory::allocator::countBlocks(bootInfo.kernelResourcesSize) * 0x1000);
	//memory::pageTableManager.mapMemory((void*)framebuffer->baseAddress, (void*)framebuffer->baseAddress, framebuffer->bufferSize);
	//loadGDT(memory::PLM4);
	//memory::initializeHeap((void*)0x0000100000000000, 0x10);
	console::printf(u8"%llx\n", &bootInfo);
	console::printf(u8"%llx %llx %llx %llx\n", memory::sizeRAM, memory::allocator::maxBlocks, memory::allocator::usedBlocks, memory::allocator::unusedBlocks);
	console::printf(u8"%llx %llx %u %u %u\n", framebuffer->baseAddress, framebuffer->bufferSize, framebuffer->width, framebuffer->height, framebuffer->pixelsPerScanline);
	//int* a = new int(5);
	//console::printf(u8"%llx", a);
	cpuio::loop();
}