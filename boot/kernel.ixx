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
import math;
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
	/*u64* pcie = reinterpret_cast<u64*>(static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x60)) | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x62)) << 16u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x64)) << 32u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x66)) << 48u);
	memory::pageTableManager.mapMemory(pcie, pcie);
	console::printf(u8"%llx\n", *pcie);*/
	double a = math::sin(3.14 / 4.0);
	double b = math::sin(3.14 / 2.0);
	double c = math::sin(3.14 / 6.0);
	double d = math::sin(3.14 / 12.0);
	double e = math::sin(3.0 * 3.14 / 4.0);
	double f = math::sin(5.0 * 3.14 / 6.0);
	double g = math::sin(3.0);
	double h = math::sin(3.1);
	/*double a = 3.14 / 4.0;
	double b = 3.14 / 2.0;
	double c = 3.14;*/
	console::printf(u8"%f\n", 0.999999682931834620);
	console::printf(u8"%f %f %f %f %f %f %f %f\n", a, b, c, d, e, f, g, h);
	for (size_t i = 0; i < 10; i++)
	{
		console::printf(u8"%f\n", math::exp((f64)i));
	}
	while (true) cpuio::halt();
}