export module kernel;
import types;
import cpuio;
import IDT;
import ISR;
import IRQ;
import pci;
import memory;
import console;
import std;
import ACPI;
import PIT;

extern "C" void kernel_start()
{
	console::setOut(console::OUT::TELETYPE);
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	IDT::loadIDTR(&IDT::idtr);
	console::initialize();
	memory::initialize();
	//memory::printSMAP();
	PIT::setDivisor(65535);
	ACPI::RSDP* rsdp = ACPI::RSDP::find();
	ACPI::MCFGHeader* mcfg = reinterpret_cast<ACPI::MCFGHeader*>(ACPI::SDTHeader::find(reinterpret_cast<ACPI::SDTHeader*>(rsdp->RSDTAddress), u8"MCFG"));
	pci::enumeratePCI(mcfg);
	for (int i = 0; i < 100; i++)
	{
		console::printf(u8"%u ", i);
		PIT::sleep(100);
	}
	cpuio::loop();
}