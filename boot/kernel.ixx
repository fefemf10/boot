export module kernel;
import types;
import cpuio;
import IDT;
import ISR;
import IRQ;
import pci;
import memory;
import console;
import sl;
import ACPI;
import PIT;
import driver.AHCI.structures;
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
	u64* pcie = reinterpret_cast<u64*>(static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x60)) | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x62)) << 16u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x64)) << 32u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x66)) << 48u);
	memory::pageTableManager.mapMemory(pcie, pcie);
	console::printf(u8"%llx\n", *pcie);
	for (int i = 0; i < 10; i++)
	{
		console::printf(u8"%u ", i);
		PIT::sleep(100);
	}
	cpuio::loop();
}