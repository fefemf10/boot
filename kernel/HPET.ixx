export module HPET;
import types;
import SDT;
import memory.PageTableManager;
import console;
export namespace ACPI
{
	bool timerNPeriodic[32]{};
	u64 indexCurrentTimer{};
	u8 currentTimerIRQLine{};
#pragma pack(1)
	struct HPET
	{
		union GCIDStruct
		{
			struct
			{
				u8 revision;
				u8 numberTimerCapabilities : 5;
				u8 countSizeCapabilities : 1;
				u8 rsv : 1;
				u8 legacyReplacementCapabilities : 1;
				u16 vendorId;
				u32 counterClkPeriod;
			};
			u64 value;
		};
		union GCRStruct
		{
			struct
			{
				u64 enableCNF : 1;
				u64 legacyReplacementCNF : 1;
				u64 rsv : 62;
			};
			u64 value;
		};
		union MCVStruct
		{
			struct
			{
				u8 rsv0 : 1;
				u8 intType : 1;
				u8 intEnable : 1;
				u8 type : 1;
				u8 periodicInt : 1;
				u8 size : 1;
				u8 valueSet : 1;
				u8 rsv1 : 1;
				u8 mode32 : 1;
				u8 intRoute : 5;
				u8 fsbEnable : 1;
				u8 fsbIntDel : 1;
				u16 rsv2;
				u32 intRouteCapability;
			};
			u64 value;
		};
		enum Registers
		{
			GCID = 0x00,
			GCR = 0x10,
			GISR = 0x20,
			MCV = 0xF0,
		};
		SDT header;
		u8 hardware_rev_id;
		u8 comparator_count : 5;
		u8 counter_size : 1;
		u8 rsv0 : 1;
		u8 legacy_replacement : 1;
		u16 pci_vendor_id;
		u8 address_space_id;
		u8 register_bit_width;
		u8 register_bit_offset;
		u8 rsv1;
		u64 address;
		u8 hpet_number;
		u16 minimum_tick;
		u8 page_protection;
		void initialize()
		{
			memory::pageTableManager.mapMemory(reinterpret_cast<const void*>(address), reinterpret_cast<const void*>(address), memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE);
			ACPI::hpet->disable();
			MCVStruct mcv;
			for (size_t i = 0; i < comparator_count + 1; i++)
			{
				mcv.value = *reinterpret_cast<u64*>(address + 0x100 + 0x20 * i);
				mcv.intEnable = false;
				writeTimerN(i, mcv);
				timerNPeriodic[i] = mcv.periodicInt & mcv.size;
			}
			for (size_t i = 0; i < comparator_count + 1; i++)
			{
				if (timerNPeriodic[i])
				{
					mcv = readTimerN(i);
					mcv.type = true;
					for (size_t j = 0; j < 32; j++)
					{
						if (((mcv.intRouteCapability >> j) & 1) && j > 2 && j != 8)
						{
							mcv.intRoute = j; //enable IRQ0
							mcv.valueSet = true;
							currentTimerIRQLine = j;
							indexCurrentTimer = i;
							writeTimerN(i, mcv);
							return;
						}
					}
				}
			}
		}
		GCIDStruct getGCID()
		{
			return GCIDStruct{ .value = read(ACPI::HPET::Registers::GCID) };
		}
		GCRStruct getGCR()
		{
			return GCRStruct{ .value = read(ACPI::HPET::Registers::GCR) };
		}
		void writeGCID(GCIDStruct gcid)
		{
			write(Registers::GCID, gcid.value);
		}
		u64 read(u16 reg)
		{
			return *reinterpret_cast<u64*>(address + reg);
		}
		void write(u16 reg, u64 value)
		{
			*reinterpret_cast<u64*>(address + reg) = value;
		}
		void enableTimerN(u8 n)
		{
			if (n > comparator_count + 1) return;
			MCVStruct mcv{ .value = *reinterpret_cast<u64*>(address + 0x100 + 0x20 * n) };
			mcv.intEnable = true;
			*reinterpret_cast<u64*>(address + 0x100 + 0x20 * n) = mcv.value;
		}
		MCVStruct readTimerN(u8 n)
		{
			if (n > comparator_count + 1) return MCVStruct();
			MCVStruct mcv{ .value = *reinterpret_cast<u64*>(address + 0x100 + 0x20 * n) };
			timerNPeriodic[n] = mcv.periodicInt & mcv.size;
			return mcv;
		}
		void writeTimerN(u8 n, MCVStruct mcv)
		{
			if (n > comparator_count + 1) return;
			*reinterpret_cast<u64*>(address + 0x100 + 0x20 * n) = mcv.value;
		}
		u64 readMainTimer()
		{
			return *reinterpret_cast<u64*>(address + Registers::MCV);
		}
		u64 readTimerComparatorN(u8 n)
		{
			if (n > comparator_count + 1) return 0;
			return *reinterpret_cast<u64*>(address + 0x108 + 0x20 * n);
		}
		void writeTimerComparatorN(u8 n, u64 value)
		{
			if (n > comparator_count + 1) return;
			*reinterpret_cast<u64*>(address + 0x108 + 0x20 * n) = value;
		}
		void disable()
		{
			write(Registers::GCR, 0b00);
		}
		void enable()
		{
			write(Registers::GCR, 0b01);
		}
	} *hpet;
	
}