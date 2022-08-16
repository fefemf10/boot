export module pci;
import types;
import cpuio;
import string;
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
#pragma pack(1)
	struct Header0
	{
		u16 vendor;
		u16 device;
		u16 cmd;
		u16 status;
		u8 revision;
		u8 programmingInterface;
		u8 subClassCode;
		u8 baseClassCode;
		u8 cacheLineSize;
		u8 masterLatencyTimer;
		u8 headerType;
		u8 builtInSeltTest;
		u32 bars[5];
		u32 bars5;
		u32 reversed;
		u16 subSystemVendor;
		u16 subSystemID;
		u32 expansionROM;
		u8 capabilityPointer;
		u32 reserved2;
		u16 reserved3;
		u8 reserved4;
		u8 interruptLine;
		u8 interruptPin;
		u8 minGrant;
		u8 maxLatency;
	};
	u8 countHeaders{};
	Header0 h[256]{};
	u16 configRealWord(u8 bus, u8 device, u8 func, u8 offset)
	{
		u32 address = (bus << 16u) | (device << 11u) | (func << 8u) | (offset & 0xFC) | 0x80000000;
		cpuio::outdw(address, configAddress);
		return cpuio::indw(configData) >> ((offset & 2) << 3) & 0xFFFF;
	}
	u16 getHeaderField(u8 bus, u8 device, u8 func, Field field)
	{
		return configRealWord(bus, device, func, static_cast<u8>(field));
	}
	void readHeader(Header0& header, u8 bus, u8 device, u8 func)
	{
		for (u8 i = 0; i < 32; i++)
		{
			reinterpret_cast<u16*>(&header)[i] = configRealWord(bus, device, func, i * 2u);
		}
	}
	void checkFunction(u8 bus, u8 device, u8 func)
	{
		u8 subClassCode = getHeaderField(bus, device, func, Field::SUBCLASSCODE) & 0xFF;
		u8 baseClassCode = getHeaderField(bus, device, func, Field::BASECLASSCODE) & 0xFF;
		if (true)
		{
			readHeader(h[countHeaders], bus, device, func);
			countHeaders++;
		}
		/*if ((baseClassCode == 0x06) && (subClassCode == 0x04))
		{
			u8 secondaryBus = getHeaderField(bus, device, func);
			checkBus(secondaryBus);
		}*/
	}
	void checkDevice(u8 bus, u8 device)
	{
		u8 func{};
		u16 vendor = getHeaderField(bus, device, func, Field::VENDOR);
		if (vendor == 0xFFFF) return;
		checkFunction(bus, device, func);
		u8 headerType = getHeaderField(bus, device, func, Field::HEADERTYPE) & 0xFF;
		if ((headerType & 0x80) != 0)
		{
			for (func = 1; func < 8; func++)
			{
				if (getHeaderField(bus, device, func, Field::VENDOR) != 0xFFFF)
				{
					checkFunction(bus, device, func);
				}
			}
		}
	}
	void checkBus(u8 bus)
	{
		for (u8 device = 0; device < 32; device++)
		{
			checkDevice(bus, device);
		}
	}
	void checkAllBuses()
	{
		u8 headerType = getHeaderField(0, 0, 0, Field::HEADERTYPE) & 0xFF;
		if ((headerType & 0x80) == 0)
		{
			checkBus(0);
		}
		else
		{
			for (u8 func = 0; func < 8; func++)
			{
				if (getHeaderField(0, 0, func, Field::VENDOR) != 0xFFFF) break;
				checkBus(func);
			}
		}
	}
}