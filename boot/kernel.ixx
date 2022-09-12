﻿export module kernel;
import types;
import cpuio;
import IDT;
import ISR;
import IRQ;
import pci;
import memory;
import console;
import hash;
void print_header(const pci::Header0& header)
{
	//teletype::puth(header);
	console::printf(u8"vendor: %s device: %s status: %hx %s\n", pci::getVendorName(header.vendor), pci::getDeviceName(header.vendor, header.device), header.status, pci::deviceClasses[header.baseClassCode]);
	/*teletype::printf("command: %hx\n", header.cmd);
	teletype::printf("status: %hx\n", header.status);
	teletype::printf("revision: %hx\n", header.revision & 0xFF);*/

	//teletype::printf("cacheLineSize: %hx\n", header.cacheLineSize & 0xFF);
	//teletype::printf("masterLatencyTimer: %hx\n", header.masterLatencyTimer & 0xFF);
	//console::printf(u8"headerType: %hx\n", header.headerType & 0xFF);
	//teletype::printf("builtInSeltTest: %hx\n", header.builtInSeltTest & 0xFF);
}
extern "C" void kernel_start()
{
	console::setOut(console::OUT::TELETYPE);
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	IDT::loadIDTR(&IDT::idtr);
	console::initialize();
	memory::initialize();
	const void* adr = (const void*)0xB8000;
	console::puth(adr, 64);
	cpuio::halt();
}