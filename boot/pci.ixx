export module pci;
import types;
import cpuio;
export namespace pci
{
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
			for (func = 1; func < 8 ; func++)
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