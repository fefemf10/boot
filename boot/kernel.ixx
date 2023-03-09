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
import glm.vec4;
import glm.mat2x2;
import glm.mat4x4;
import glm.transform;
import sl.math;
import sl.numbers;
import sl.vector;
import SMBIOS;
import memory.utils;
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
	//std::vector<int> a;
	////a.reserve(8);
	//console::printf(u8"%llx %i %i\n", a.data(), a[0], a[1]);
	//console::puth(a.data(), sizeof(int) * a.capacity());
	//a.reserve(16);
	//console::printf(u8"%llx %i %i\n", a.data(), a[0], a[1]);
	//console::puth(a.data(), sizeof(int) * a.capacity());
	/*size_t i = 0;
	for (; i < VESA::countModes; i++)
	{
		console::printf(u8"%hu %hu %hu %hu %x\n", VESA::vesaModesInfo[i].width, VESA::vesaModesInfo[i].height, VESA::vesaModesInfo[i].attributes, VESA::vesaModesInfo[i].bpp, VESA::vesaModesInfo[i].framebuffer);
	}
	i = VESA::currentMode;
	console::printf(u8"%hu %hu %hu %hu %x\n", VESA::vesaModesInfo[i].width, VESA::vesaModesInfo[i].height, VESA::vesaModesInfo[i].attributes, VESA::vesaModesInfo[i].bpp, VESA::vesaModesInfo[i].framebuffer);
	console::printf(u8"%hx\n", VESA::currentMode);*/
	/*SMBIOS::SMBIOS* smbios = SMBIOS::SMBIOS::find();
	if (smbios)
	{
		SMBIOS::SMBIOSHeaderStruct* header = reinterpret_cast<SMBIOS::SMBIOSHeaderStruct*>(smbios->V2.structureTableAddress);
		while (header->type != std::to_underlying(SMBIOS::Type::PROCESSOR_INFORMATION))
		{
			header = reinterpret_cast<SMBIOS::SMBIOSHeaderStruct*>(smbios->V2.structureTableAddress)
		}
		console::printf(u8"%hx\n", smbios->V2.numberOfStructures);
	}*/
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
	
	PIT::setDivisor(65535);
	glm::f32mat4x4 r = glm::rotate(glm::f32mat4x4(1.f), std::numbers::pi_v<float> / 12.f / 16.f, (glm::f32vec3(1.f, 1.f, 1.f)));
	double dt = 0;
	while (true)
	{
		//double start = PIT::timeSinceBoot;
		VESA::clear();
		for (size_t i = 0; i < 12; i++)
		{
			int x0 = vertex[edge[i].x].x + 520;
			int y0 = 440 - vertex[edge[i].x].y;
			int x1 = vertex[edge[i].y].x + 520;
			int y1 = 440 - vertex[edge[i].y].y;
			VESA::drawLine(x0, y0, x1, y1, 0xFF00FF00);
		}
		VESA::drawRectangle(20, 20, 100, 100, 0xFF00FF00);
		VESA::drawCircle(200, 200, 100, 0xFF0000FF);
		for (size_t i = 0; i < 8; i++)
		{
			glm::f32vec4 m = r * glm::f32vec4(vertex[i], 1.f);
			vertex[i] = glm::f32vec3(m);
		}
		VESA::swap();
		/*double end = PIT::timeSinceBoot;
		dt = end - start;
		console::printf(u8"%f\n", dt);*/
	}
	//memory::printSMAP();
	
	/*ACPI::RSDP* rsdp = ACPI::RSDP::find();
	ACPI::MCFGHeader* mcfg = reinterpret_cast<ACPI::MCFGHeader*>(ACPI::SDTHeader::find(reinterpret_cast<ACPI::SDTHeader*>(rsdp->RSDTAddress), u8"MCFG"));
	pci::enumeratePCI(mcfg);*/
	/*u64* pcie = reinterpret_cast<u64*>(static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x60)) | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x62)) << 16u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x64)) << 32u | static_cast<u64>(pci::old::configRealWord(0, 0, 0, 0x66)) << 48u);
	memory::pageTableManager.mapMemory(pcie, pcie);
	console::printf(u8"%llx\n", *pcie);*/
	cpuio::loop();
}