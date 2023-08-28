export module IOAPIC;
import types;
import memory.allocator;
import memory.PageTableManager;

export namespace APIC
{
	struct IOAPIC
	{
		enum class DeliveryMode : u64
		{
			EDGE,
			LEVEL
		};
		enum class DesctinationMode : u64
		{
			PHYSICAL,
			LOGICAL
		};
		enum Registers
		{
			ID,
			VERSION,
			ARBITRATION,
			REDIRECTION_TABLE = 0x10
		};
		union REDTBLEntry
		{
			struct
			{
				u64 vector : 8;
				DeliveryMode deliveryMode : 3;
				DesctinationMode destinationMode : 1;
				u64 deliveryStatus : 1;
				u64 pinPolarity : 1;
				u64 remoteIRR : 1;
				u64 triggerMode : 1;
				u64 mask : 1;
				u64 rsv : 39;
				u64 destination : 8;
			};
			struct
			{
				u32 lower;
				u32 upper;
			};
		};
		IOAPIC()
		{

		}
		IOAPIC(void* physicalAddress, u32 GSIBase) : physicalAddress(physicalAddress)
		{
			virtualAddress = physicalAddress;
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::MemoryFlagsBits(memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE));
			id = (read(Registers::ID) >> 24) & 0xF;
			version = read(Registers::VERSION);
			redirectionSize = (read(Registers::VERSION) >> 16) + 1;
			this->GSIBase = GSIBase;
		}
		void initialize(void* physicalAddress, u32 GSIBase)
		{
			this->physicalAddress = physicalAddress;
			virtualAddress = physicalAddress;
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::MemoryFlagsBits(memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE));
			id = (read(Registers::ID) >> 24) & 0xF;
			version = read(Registers::VERSION);
			redirectionSize = (read(Registers::VERSION) >> 16) + 1;
			this->GSIBase = GSIBase;
		}
		REDTBLEntry readREDTBL(u8 gsi)
		{
			REDTBLEntry entry{};
			if (gsi >= redirectionSize)
				return entry;
			gsi *= 2;
			gsi += REDIRECTION_TABLE;
			entry.lower = read(gsi);
			entry.upper = read(gsi + 1);
			return entry;
		}
		void writeREDTBL(u8 gsi, const REDTBLEntry& entry)
		{
			if (gsi >= redirectionSize)
				return;
			gsi *= 2;
			gsi += REDIRECTION_TABLE;
			write(gsi, entry.lower);
			write(gsi + 1, entry.upper);
		}
		void setMaskGSI(u8 gsi, bool mask)
		{
			REDTBLEntry entry = readREDTBL(gsi);
			entry.mask = mask;
			writeREDTBL(gsi, entry);
		}
		void* physicalAddress;
		void* virtualAddress;
		u8 id;
		u8 version;
		u8 redirectionSize;
		u8 rsv;
		u32 GSIBase;
		u32 read(u8 reg)
		{
			*(u32 volatile*)virtualAddress = reg;
			return *(u32 volatile*)(reinterpret_cast<u64>(virtualAddress) + 0x10);
		}
		void write(u8 reg, u32 value)
		{
			*(u32 volatile*)virtualAddress = reg;
			*(u32 volatile*)(reinterpret_cast<u64>(virtualAddress) + 0x10) = value;
		}
	};
}