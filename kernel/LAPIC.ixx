export module LAPIC;
import types;
import memory.allocator;
import memory.PageTableManager;
import memory.utils;
import memory.flags;

export namespace APIC
{
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
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::MemoryFlagsBits(memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE));
			id = read(Registers::ID) >> 24;
			version = read(Registers::VERSION);
			countLVT = (read(Registers::VERSION) >> 16) + 1;
		}
		void initialize(u8 id, u8 processordId)
		{
			this->id = id;
			this->processorId = processorId;
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
		u8 version;
		u8 countLVT;
	};
}