export module kernel;
import types;
import cpuio;
import IDT;
import ISR;
import IRQ;
import teletype;
//void print_header(const pci::Header0& header)
//{
//	//teletype::puth(header);
//	teletype::printf("vendor: %hx\n", header.vendor);
//	teletype::printf("device: %hx\n", header.device);/*
//	teletype::printf("command: %hx\n", header.cmd);
//	teletype::printf("status: %hx\n", header.status);
//	teletype::printf("revision: %hx\n", header.revision & 0xFF);*/
//	teletype::printf("programmingInterface: %hx\n", header.programmingInterface & 0xFF);
//	teletype::printf("subClassCode: %hx\n", header.subClassCode & 0xFF);
//	teletype::printf("baseClassCode: %hx\n", header.baseClassCode & 0xFF);
//	/*teletype::printf("cacheLineSize: %hx\n", header.cacheLineSize & 0xFF);
//	teletype::printf("masterLatencyTimer: %hx\n", header.masterLatencyTimer & 0xFF);
//	teletype::printf("headerType: %hx\n", header.headerType & 0xFF);
//	teletype::printf("builtInSeltTest: %hx\n", header.builtInSeltTest & 0xFF);*/
//}
extern "C" void kernel_start()
{
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	IDT::loadIDTR(&IDT::idtr);
	teletype::clear();
	/*pci::checkAllBuses();
	teletype::printf("%d", pci::countHeaders);
	for (u8 i = 0; i < pci::countHeaders; i++)
	{
		print_header(pci::h[i]);
	}*/
}