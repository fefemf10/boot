export module kernel;
import types;
import cpuio;
import IDT;
import ISR;
import IRQ;
import pci;
import memory;
import memory.allocator;
import memory.SMAP;
import console;
import sl;
import ACPI;
import PIT;
import VESA;
import driver.AHCI.structures;
import glm;
import glm.vec2;
import glm.vec3;
import glm.mat2x2;
extern "C" void kernel_start()
{
	IDT::initialize();
	ISR::initialize();
	IRQ::initialize();
	IDT::loadIDTR(&IDT::idtr);
	console::initialize();
	console::setOut(console::OUT::SERIAL);
	memory::initialize();
	VESA::initialize();
	//console::printf(u8"%c %s %i %lli %llu %% %x %f %.2f\n",
		//u8'A', "Hello", -5, -48889845131554, -48889845131554, 0xABCDEF, 2.84898878f, 2.84898878f);
	console::setOut(console::OUT::SERIAL);
	size_t i = 0;
	for (; i < VESA::countModes; i++)
	{
		console::printf(u8"%hu %hu %hu %hu %x\n", VESA::vesaModesInfo[i].width, VESA::vesaModesInfo[i].height, VESA::vesaModesInfo[i].attributes, VESA::vesaModesInfo[i].bpp, VESA::vesaModesInfo[i].framebuffer);
	}
	i = VESA::currentMode;
	console::printf(u8"%hu %hu %hu %hu %x\n", VESA::vesaModesInfo[i].width, VESA::vesaModesInfo[i].height, VESA::vesaModesInfo[i].attributes, VESA::vesaModesInfo[i].bpp, VESA::vesaModesInfo[i].framebuffer);
	console::printf(u8"%hx\n", VESA::currentMode);
	glm::f32vec3 vertex[8] =
	{
		{0, 0, 0},
		{0, 110, 0},
		{175, 110, 0},
		{175, 0, 0},
		{0, 0, 150},
		{0, 110, 150},
		{175, 110, 150},
		{175, 0, 150}
	};
	glm::u8vec2 edge[12] =
	{
		{0, 1},
		{1, 2},
		{2, 3},
		{3, 0},
		{4, 5},
		{5, 6},
		{6, 7},
		{7, 4},
		{0, 4},
		{1, 5},
		{2, 6},
		{3, 7}
	};
	glm::f32mat2x2 s(1, 2, 3, 4);
	glm::f32mat2x2 f = s.transpose();
	console::printf(u8"%u %u %u %u\n", s[0][0], s[0][1], s[1][0], s[1][1]);
	console::printf(u8"%u %u %u %u\n", f[0][0], f[0][1], f[1][0], f[1][1]);
	while (true)
	{
		for (size_t i = 0; i < 12; i++)
		{
			int x0 = vertex[edge[i].x].x + 320;
			int y0 = 240 - vertex[edge[i].x].y;
			int x1 = vertex[edge[i].y].x + 320;
			int y1 = 240 - vertex[edge[i].y].y;
			VESA::drawLine(x0, y0, x1, y1, 128, 255, 255);
		}
		PIT::sleep(16);
	}
	//memory::printSMAP();
	//PIT::setDivisor(65535);
	//ACPI::RSDP* rsdp = ACPI::RSDP::find();
	//ACPI::MCFGHeader* mcfg = reinterpret_cast<ACPI::MCFGHeader*>(ACPI::SDTHeader::find(reinterpret_cast<ACPI::SDTHeader*>(rsdp->RSDTAddress), u8"MCFG"));
	//pci::enumeratePCI(mcfg);
	/*u64* pcie = reinterpret_cast<u64*>(static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x60)) | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x62)) << 16u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x64)) << 32u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x66)) << 48u);
	memory::pageTableManager.mapMemory(pcie, pcie);
	console::printf(u8"%llx\n", *pcie);*/
	//double a = math::sin(3.14 / 4.0);
	//double b = math::sin(3.14 / 2.0);
	//double c = math::sin(3.14 / 6.0);
	//double d = math::sin(3.14 / 12.0);
	//double e = math::sin(3.0 * 3.14 / 4.0);
	//double f = math::sin(5.0 * 3.14 / 6.0);
	//double g = math::sin(3.0);
	//double h = math::sin(3.1);
	/*double a = 3.14 / 4.0;
	double b = 3.14 / 2.0;
	double c = 3.14;*/
	//console::printf(u8"%f\n", 0.999999682931834620);
	//console::printf(u8"%f %f %f %f %f %f %f %f\n", a, b, c, d, e, f, g, h);
	/*u64* code = reinterpret_cast<u64*>(memory::allocator::allocBlocks(1));
	*code = 0xC3C889C1FF;
	int (*compiledFun)(int) = reinterpret_cast<int(*)(int)>(code);*/
	//int n = compiledFun(0);
	//console::printf(u8"n = %i", n);
	//console::setOut(console::OUT::SERIAL);
	//for (size_t i = 0; i < 100000; i++)
	//{
	//	console::printf(u8"%lli %f\n", i, math::pow((f64)i, 0.5));
	//}
	/*double x = 2.0;
	double es[] = { 0.0,1.0,2.0,6.0,24.0,120.0,720.0,5040.0,40320.0,362880.0,3628800.0,39916800.0,479001600.0,6227020800.0,87178291200.0,1307674368000.0,20922789888000.0 };
	double s[17]{};
	double n = 0.0;
	for (size_t i = 1; i < 16; i++)
	{
		double t = 1.0;
		for (size_t j = 0; j < i; j++)
		{
			t *= x;
		}
		s[i] = t / es[i];
		n += s[i];
	}
	console::printf(u8"%f ", cexp(9, 12));*/
	cpuio::loop();
}