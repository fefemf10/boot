export module kernel;
import types;
import cpuio;
import IDT;
import ISR;
import IRQ;
import teletype;
import pci;
import memory;
void print_header(const pci::Header0& header)
{
	//teletype::puth(header);
	teletype::printf("vendor: %hx device: %hx PI: %hx SCC: %hx BCC: %hx\n", header.vendor, header.device, header.programmingInterface & 0xFF, header.subClassCode & 0xFF, header.baseClassCode & 0xFF);
	/*teletype::printf("command: %hx\n", header.cmd);
	teletype::printf("status: %hx\n", header.status);
	teletype::printf("revision: %hx\n", header.revision & 0xFF);*/

	/*teletype::printf("cacheLineSize: %hx\n", header.cacheLineSize & 0xFF);
	teletype::printf("masterLatencyTimer: %hx\n", header.masterLatencyTimer & 0xFF);
	teletype::printf("headerType: %hx\n", header.headerType & 0xFF);
	teletype::printf("builtInSeltTest: %hx\n", header.builtInSeltTest & 0xFF);*/
}
extern "C" void kernel_start()
{
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	IDT::loadIDTR(&IDT::idtr);
	teletype::clear();
	memory::printSMAP();
	memory::set(reinterpret_cast<u64*>(0x7c00), 0, 0x200);
	/*memory::PageTable* PLM4 = reinterpret_cast<memory::PageTable*>(memory::getPage());
	memory::set(PLM4, 0, 0x1000);
	memory::PageTableManager pageTableManager(PLM4);
	for (u64 i = 0; i < length; i++)
	{

	}*/
	pci::checkAllBuses();
	for (u8 i = 0; i < pci::countHeaders; i++)
	{
		print_header(pci::h[i]);
	}
	cpuio::halt();
}