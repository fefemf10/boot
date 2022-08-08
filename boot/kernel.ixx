export module kernel;
import types;
import cpuio;
import IDT;
import ISR;
import IRQ;
import pci;
import memory;
import console;
import teletype;
import serial;
void print_header(const pci::Header0& header)
{
	//teletype::puth(header);
	console::printf("vendor: %hx device: %hx PI: %hx SCC: %hx BCC: %hx\n", header.vendor, header.device, header.programmingInterface & 0xFF, header.subClassCode & 0xFF, header.baseClassCode & 0xFF);
	/*teletype::printf("command: %hx\n", header.cmd);
	teletype::printf("status: %hx\n", header.status);
	teletype::printf("revision: %hx\n", header.revision & 0xFF);*/

	/*teletype::printf("cacheLineSize: %hx\n", header.cacheLineSize & 0xFF);
	teletype::printf("masterLatencyTimer: %hx\n", header.masterLatencyTimer & 0xFF);
	teletype::printf("headerType: %hx\n", header.headerType & 0xFF);
	teletype::printf("builtInSeltTest: %hx\n", header.builtInSeltTest & 0xFF);*/
}
extern "C" void loadGDT(memory::PageTable* plm4);
extern "C" void kernel_start()
{
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	IDT::loadIDTR(&IDT::idtr);
	teletype::clear();
	serial::initialize();
	memory::initialize();
	console::setOut(console::OUT::SERIAL);

	memory::printSMAP();
	memory::PageTable* PLM4 = reinterpret_cast<memory::PageTable*>(memory::allocator::allocBlocks(1));
	memory::set(PLM4, 0, 0x1000);
	memory::PageTableManager pageTableManager(PLM4);
	u32 size = *reinterpret_cast<u32*>(0x27A00);
	memory::SMAP* smap = reinterpret_cast<memory::SMAP*>(0x27A04);
	u64 totalRAM = reinterpret_cast<u64>(smap[size - 1].base) + smap[size - 1].length - 1ull;
	for (u64 i = 0; i < 0x40000; i += 0x1000)
	{
		pageTableManager.mapMemory((void*)i, (void*)i);
	}
	loadGDT(PLM4);
	console::printf("memory");
	/*pci::checkAllBuses();
	for (u8 i = 0; i < pci::countHeaders; i++)
	{
		print_header(pci::h[i]);
	}*/
	cpuio::halt();
}