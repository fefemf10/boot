export module pci;
export import pci.structures;
import types;
import string;
import ACPI;
import memory;
import driver.AHCI;
import console;
export namespace pci
{
	const char8_t* deviceClasses[] =
	{
		u8"Unclassified",
		u8"Mass Storage Controller",
		u8"Network Controller",
		u8"Display Controller",
		u8"Multimedia Controller",
		u8"Memory Controller",
		u8"Bridge",
		u8"Simple Communication Controller",
		u8"Base System Peripheral",
		u8"Input Device Controller",
		u8"Docking Station",
		u8"Processor",
		u8"Serial Bus Controller",
		u8"Wireless Controller",
		u8"Intelligent Controller",
		u8"Satellite Communication Controller",
		u8"Encryption Controller",
		u8"Signal Processing Controller",
		u8"Processing Accelerator",
		u8"Non-Essential Instrumentation",
	};
	constexpr const char8_t* getVendorName(u16 vendor)
	{
		switch (vendor)
		{
		case 0x8086:
			return u8"Intel Corp.";
		case 0x1022:
			return u8"Advanced Micro Devices, Inc. [AMD]";
		case 0x10DE:
			return u8"NVIDIA Corporation";
		case 0x15AD:
			return u8"VMware";
		case 0x1002:
			return u8"Advanced Micro Devices, Inc. [AMD/ATI]";
		default:
			return string::itos(vendor, 16);
		}
	}
	constexpr const char8_t* getDeviceName(u16 vendor, u16 device)
	{
		switch (vendor)
		{
		case 0x8086:
			switch (device)
			{
			case 0x10D3:
				return u8"82574L Gigabit Network Connection";
			case 0x1237:
				return u8"440FX - 82441FX PMC [Natoma]";
			case 0x7000:
				return u8"82371SB PIIX3 ISA [Natoma/Triton II]";
			case 0x7010:
				return u8"82371SB PIIX3 IDE [Natoma/Triton II]";
			case 0x7190:
				return u8"440BX/ZX/DX - 82443BX/ZX/DX Host bridge";
			case 0x7191:
				return u8"440BX/ZX/DX - 82443BX/ZX/DX AGP bridge";
			case 0x7110:
				return u8"82371AB/EB/MB PIIX4 ISA";
			case 0x7111:
				return u8"82371AB/EB/MB PIIX4 IDE";
			case 0x7113:
				return u8"82371AB/EB/MB PIIX4 ACPI";
			case 0x100E:
				return u8"82540EM Gigabit Ethernet Controller";
			case 0x29C0:
				return u8"82G33/G31/P35/P31 Express DRAM Controller";
			case 0x2918:
				return u8"82801IB (ICH9) LPC Interface Controller";
			case 0x2922:
				return u8"82801IR/IO/IH(ICH9R/DO/DH) 6 port SATA Controller[AHCI mode]";
			case 0x2930:
				return u8"82801I (ICH9 Family) SMBus Controller";
			default:
				return string::itos(device, 16);
			}
		case 0x15AD:
			switch (device)
			{
			case 0x0405:
				return u8"SVGA II Adapter";
			case 0x0710:
				return u8"SVGA Adapter";
			case 0x0720:
				return u8"VMXNET Ethernet Controller";
			case 0x0740:
				return u8"Virtual Machine Communication Interface";
			case 0x0770:
				return u8"USB2 EHCI Controller";
			case 0x0774:
				return u8"USB1.1 UHCI Controller";
			case 0x0778:
				return u8"USB3 xHCI 0.96 Controller";
			case 0x0779:
				return u8"USB3 xHCI 1.0 Controller";
			case 0x0790:
				return u8"PCI bridge";
			case 0x07A0:
				return u8"PCI Express Root Port";
			case 0x07b0:
				return u8"VMXNET3 Ethernet Controller";
			case 0x07c0:
				return u8"PVSCSI SCSI Controller";
			case 0x07e0:
				return u8"SATA AHCI controller";
			case 0x0801:
				return u8"Virtual Machine Interface";
			case 0x0820:
				return u8"Paravirtual RDMA controller";
			case 0x1977:
				return u8"HD Audio Controller";
			default:
				return string::itos(device, 16);
			}
		case 0x1022:
			switch (device)
			{
			case 0x9600:
				return u8"RS780 Host Bridge";
			case 0x9603:
				return u8"RS780 PCI to PCI bridge (ext gfx port 0)";
			case 0x9604:
				return u8"RS780/RS880 PCI to PCI bridge (PCIE port 0)";
			}
		case 0x1002:
			switch (device)
			{
			case 0x1200:
				return u8"Family 10h Processor HyperTransport Configuration";
			case 0x1201:
				return u8"Family 10h Processor Address Map";
			case 0x1202:
				return u8"Family 10h Processor DRAM Controller";
			case 0x1203:
				return u8"Family 10h Processor Miscellaneous Control";
			case 0x1204:
				return u8"Family 10h Processor Link Control";
			}
		default:
			return string::itos(device, 16);
		}
	}
	constexpr const char8_t* getClassName(u8 classCode)
	{
		switch (classCode)
		{
		case 0x06:
				return u8"Bridge";
		default:
			return string::itos(classCode, 16);
		}
	}
	constexpr const char8_t* getSubclassName(u8 classCode, u8 subclassCode)
	{
		switch (classCode)
		{
		case 0x01:
			switch (subclassCode)
			{
			case 0x00:
				return u8"SCSI Bus Controller";
			case 0x01:
				return u8"IDE Controller";
			case 0x02:
				return u8"Floppy Disk Controller";
			case 0x03:
				return u8"IPI Bus Controller";
			case 0x04:
				return u8"RAID Controller";
			case 0x05:
				return u8"ATA Controller";
			case 0x06:
				return u8"Serial ATA Controller";
			case 0x07:
				return u8"Serial Attached SCSI Controller";
			case 0x08:
				return u8"Non-Volatile Memory Controller";
			case 0x80:
				return u8"Other";
			default:
				return string::itos(subclassCode, 16);
			}
		case 0x06:
			switch (subclassCode)
			{
			case 0x00:
				return u8"Host Bridge";
			case 0x06:
				return u8"NuBus Bridge";
			default:
				return string::itos(subclassCode, 16);
			}
		default:
			return string::itos(classCode, 16);
		}
	}
	constexpr const char8_t* getProgIFName(u8 classCode, u8 subclassCode, u8 progIF)
	{
		switch (classCode)
		{
		case 0x01:
			switch (subclassCode)
			{
			case 0x06:
				switch (progIF)
				{
				case 0x00:
					return u8"Vendor Specific Interface";
				case 0x01:
					return u8"AHCI 1.0";
				case 0x02:
					return u8"Serial Storage Bus";
				default:
					string::itos(progIF, 16);
				}
			default:
				return string::itos(subclassCode, 16);
			}
		default:
			return string::itos(classCode, 16);
		}
	}
	constexpr u16 configAddress = 0x0CF8;
	constexpr u16 configData = 0x0CFC;
	enum class Field : u8
	{
		VENDOR = 0x00,
		DEVICE = 0x02,
		COMMAND = 0x04,
		STATUS = 0x06,
		REVISION = 0x08,
		PROGRAMMINGINTERFACE = 0x09,
		SUBCLASSCODE = 0x0A,
		BASECLASSCODE = 0x0B,
		CACHELINESIZE = 0x0C,
		MASTERLATENCYTIMER = 0x0D,
		HEADERTYPE = 0x0E,
		BITS = 0x0F,
	};

	void enumerateFunction(u64 deviceAddress, u64 function)
	{
		u64 offset = function << 15;
		u64 functionAddress = deviceAddress + offset;
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(functionAddress), reinterpret_cast<void*>(functionAddress));
		Header* deviceHeader = reinterpret_cast<Header*>(functionAddress);
		if (deviceHeader->vendor == 0) return;
		if (deviceHeader->vendor == 0xFFFF) return;
		
		//console::printf(u8"%s %s %s %s %s\n\n", getVendorName(deviceHeader->vendor), getDeviceName(deviceHeader->vendor, deviceHeader->device),
			//getClassName(deviceHeader->classCode), getSubclassName(deviceHeader->classCode, deviceHeader->subclassCode), getProgIFName(deviceHeader->classCode, deviceHeader->subclassCode, deviceHeader->programmingInterface));
		switch (deviceHeader->classCode)
		{
		case 0x01:
			switch (deviceHeader->subclassCode)
			{
			case 0x06:
				switch (deviceHeader->programmingInterface)
				{
				case 0x02:
					new driver::AHCI(deviceHeader);
					return;
				default:
					break;
				}
			default:
				break;
			}
		default:
			break;
		}
	}
	void enumerateDevice(u64 busAddress, u64 device)
	{
		u64 offset = device << 15;
		u64 deviceAddress = busAddress + offset;
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(deviceAddress), reinterpret_cast<void*>(deviceAddress));
		Header* deviceHeader = reinterpret_cast<Header*>(deviceAddress);
		if (deviceHeader->vendor == 0) return;
		if (deviceHeader->vendor == 0xFFFF) return;

		for (size_t i = 0; i < 32; i++)
		{
			enumerateFunction(deviceAddress, i);
		}
	}
	void enumerateBus(u64 baseAddress, u64 bus)
	{
		u64 offset = bus << 20;
		u64 busAddress = baseAddress + offset;
		memory::pageTableManager.mapMemory(reinterpret_cast<void*>(busAddress), reinterpret_cast<void*>(busAddress));
		Header* deviceHeader = reinterpret_cast<Header*>(baseAddress);
		if (deviceHeader->vendor == 0) return;
		if (deviceHeader->vendor == 0xFFFF) return;
		
		for (size_t i = 0; i < 32; i++)
		{
			enumerateDevice(busAddress, i);
		}
	}
	void enumeratePCI(ACPI::MCFGHeader* mcfg)
	{
		size_t entries = (mcfg->header.length - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);
		ACPI::DeviceConfig* deviceConfigs = reinterpret_cast<ACPI::DeviceConfig*>(reinterpret_cast<u64>(mcfg) + sizeof(ACPI::MCFGHeader));
		for (size_t i = 0; i < entries; i++)
		{
			for (size_t j = deviceConfigs[i].startBus; j < deviceConfigs[i].endBus; j++)
			{
				console::printf(u8"%hx", j);
				enumerateBus(deviceConfigs[i].baseAddress, j);
				int a = 1;
				int b = 0;
				int c = a/b;
			}
		}
	}
}