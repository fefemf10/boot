export module APIC;
import types;
import memory.allocator;
import memory.PageTableManager;
export namespace APIC
{
	void* lapic;
	void* ioapic;
	struct LAPIC
	{
		enum Registers
		{
			ID = 0x20,
			VERSION = 0x30,
			TASK_PRIORITY = 0x80,
			ARBITRATION_PRIORITY = 0x90,
			PROCESSOR_PRIORITY = 0xA0,
			EOI = 0xB0,
			REMOTE_READ = 0xC0,
			LOGICAL_DESTINATION = 0xD0,
			DESTINATION_FORMAT = 0xE0,
			SPURIOUS_INTERRUPT_VECTOR = 0xF0,
			ERROR_STATUS = 0x280,
			LVT_CMCI = 0x2F0,
			LVT_TIMER = 0x320,
			LVT_TERMAL_SENSOR = 0x330,
			LVT_PMC = 0x340,
			LVT_LINT0 = 0x350,
			LVT_LINT1 = 0x360,
			LVT_ERROR = 0x370,
			INITIAL_COUNT = 0x380,
			CURRENT_COUNT = 0x390,
			DIVIDE_CONFIGURATION = 0x3E0,
		};
		LAPIC()
		{

		}
		LAPIC(void* physicalAddress) : physicalAddress(physicalAddress)
		{

		}
		void initialize(void* physicalAddress)
		{
			this->physicalAddress = physicalAddress;
			virtualAddress = memory::allocator::allocBlocks(1);
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::Flags((u32)memory::Flags::PRESENT | (u32)memory::Flags::READ_WRITE | (u32)memory::Flags::CACHE_DISABLE));
		}
		u32 read(Registers registerOffset)
		{
			return *(u32 volatile*)(reinterpret_cast<u64>(virtualAddress) + registerOffset);
		}
		void write(Registers registerOffset, u32 value)
		{
			*(u32 volatile*)(reinterpret_cast<u64>(virtualAddress) + registerOffset) = value;
		}
		void* physicalAddress;
		void* virtualAddress;
		u8 id;
		u8 processorId;
	};
	struct IOAPIC
	{
		enum class DeliveryMode
		{
			EDGE,
			LEVEL
		};
		enum class DesctinationMode
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
		union RedirectionEntry
		{
			struct
			{
				u64 vector : 8;
				u64 deliveryMode : 3;
				u64 destinationMode : 1;
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
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::Flags((u32)memory::Flags::PRESENT | (u32)memory::Flags::READ_WRITE | (u32)memory::Flags::CACHE_DISABLE));
			id = read(Registers::ID);
			version = read(Registers::VERSION);
			redirectionSize = (read(Registers::VERSION) >> 16) + 1;
			this->GSIBase = GSIBase;
		}
		void initialize(void* physicalAddress, u32 GSIBase)
		{
			this->physicalAddress = physicalAddress;
			virtualAddress = physicalAddress;
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::Flags((u32)memory::Flags::PRESENT | (u32)memory::Flags::READ_WRITE | (u32)memory::Flags::CACHE_DISABLE));
			id = read(Registers::ID);
			version = read(Registers::VERSION);
			redirectionSize = (read(Registers::VERSION) >> 16) + 1;
			this->GSIBase = GSIBase;
		}
		RedirectionEntry getRedirectionEntry(u8 entryIndex)
		{
			RedirectionEntry entry{};
			if (entryIndex > redirectionSize)
				return entry;
			entryIndex * 2;
			entryIndex += REDIRECTION_TABLE;
			entry.lower = read(entryIndex);
			entry.upper = read(entryIndex+1);
			return entry;
		}
		void setRedirectionEntry(u8 entryIndex, RedirectionEntry entry)
		{
			if (entryIndex > redirectionSize)
				return;
			entryIndex * 2;
			entryIndex += REDIRECTION_TABLE;
			write(entryIndex, entry.lower);
			write(entryIndex+1, entry.upper);
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
	APIC::LAPIC lapics[256];
	APIC::IOAPIC ioapics[10];
}