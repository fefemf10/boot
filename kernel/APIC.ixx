export module APIC;
import types;
import memory.allocator;
import memory.PageTableManager;
import memory.utils;
import MADT;
import PIT;
import intrinsic0;
import BootInfo;
import console;
import serial;
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
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::MemoryFlags(memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE));
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
		union RedirectionEntry
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
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::MemoryFlags(memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE));
			id = (read(Registers::ID) >> 24) & 0xF;
			version = read(Registers::VERSION);
			redirectionSize = (read(Registers::VERSION) >> 16) + 1;
			this->GSIBase = GSIBase;
		}
		void initialize(void* physicalAddress, u32 GSIBase)
		{
			this->physicalAddress = physicalAddress;
			virtualAddress = physicalAddress;
			memory::pageTableManager.mapMemory(physicalAddress, virtualAddress, memory::MemoryFlags(memory::MemoryFlagsBits::ePRESENT | memory::MemoryFlagsBits::eREAD_WRITE | memory::MemoryFlagsBits::eCACHE_DISABLE));
			id = (read(Registers::ID) >> 24) & 0xF;
			version = read(Registers::VERSION);
			redirectionSize = (read(Registers::VERSION) >> 16) + 1;
			this->GSIBase = GSIBase;
		}
		RedirectionEntry getRedirectionEntry(u8 entryIndex)
		{
			RedirectionEntry entry{};
			if (entryIndex >= redirectionSize)
				return entry;
			entryIndex *= 2;
			entryIndex += REDIRECTION_TABLE;
			entry.lower = read(entryIndex);
			entry.upper = read(entryIndex + 1);
			return entry;
		}
		void setRedirectionEntry(u8 entryIndex, RedirectionEntry entry)
		{
			if (entryIndex >= redirectionSize)
				return;
			entryIndex *= 2;
			entryIndex += REDIRECTION_TABLE;
			write(entryIndex, entry.lower);
			write(entryIndex + 1, entry.upper);
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
	u8 numlapic = 0;
	u8 numioapic = 0;
	void initialize()
	{
		numlapic = 0;
		numioapic = 0;
		APIC::lapic = reinterpret_cast<void*>(ACPI::madt->lapic);
		for (size_t i = 0; i < ACPI::madt->sizeEntries(); i++)
		{
			if (ACPI::madt->entries[i].type == ACPI::Type::IOAPIC)
			{
				ACPI::IOAPIC* ioapic = reinterpret_cast<ACPI::IOAPIC*>(&ACPI::madt->entries[i]);
				if (ioapic->length == 12)
				{
					APIC::ioapics[numioapic++].initialize(reinterpret_cast<void*>(ioapic->address), ioapic->GSIBase);
				}
			}
			if (ACPI::madt->entries[i].type == ACPI::Type::LAPICO)
			{
				ACPI::LAPICO* lapico = reinterpret_cast<ACPI::LAPICO*>(&ACPI::madt->entries[i]);
				if (lapico->length == 12)
				{
					APIC::lapic = lapico->lapic;
				}
			}
		}
		APIC::lapics[numlapic++].initialize(APIC::lapic);
		for (size_t i = 0; i < ACPI::madt->sizeEntries(); i++)
		{
			if (ACPI::madt->entries[i].type == ACPI::Type::PLAPIC)
			{
				ACPI::PLAPIC* plapic = reinterpret_cast<ACPI::PLAPIC*>(&ACPI::madt->entries[i]);
				if (plapic->length == 8 && plapic->flags == 1 && APIC::lapics[0].id != plapic->apicId)
				{
					APIC::lapics[numlapic++].initialize(plapic->apicId, plapic->processorId);
				}
			}
		}
		for (size_t i = 0; i < numioapic; i++)
		{
			for (size_t j = 0; j < APIC::ioapics[i].redirectionSize; j++)
			{
				APIC::IOAPIC::RedirectionEntry entry = APIC::ioapics[i].getRedirectionEntry(j);
				entry.mask = 1;
				APIC::ioapics[i].setRedirectionEntry(j, entry);
			}
		}
		APIC::lapics[0].write(APIC::LAPIC::SPURIOUS_INTERRUPT_VECTOR, 0x1FF);
		APIC::lapics[0].write(APIC::LAPIC::TASK_PRIORITY, 0);
		APIC::IOAPIC::RedirectionEntry PITInterrupt{};
		PITInterrupt.vector = 0x20;
		PITInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		PITInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		PITInterrupt.pinPolarity = 0;
		PITInterrupt.destination = APIC::lapics[0].id;
		PITInterrupt.mask = 0;
		APIC::ioapics[0].setRedirectionEntry(2, PITInterrupt);
		APIC::IOAPIC::RedirectionEntry keyboardInterrupt{};
		keyboardInterrupt.vector = 0x21;
		keyboardInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		keyboardInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		keyboardInterrupt.pinPolarity = 0;
		keyboardInterrupt.destination = APIC::lapics[0].id;
		keyboardInterrupt.mask = 0;
		APIC::ioapics[0].setRedirectionEntry(1, keyboardInterrupt);
	}
	extern volatile u8 aprunning = 0;  // count how many APs have started
	extern u8 bspdone = 0;      // BSP id and spinlock flag
	extern void* ap_stack = nullptr;
	extern u64 ap_stackNumberPagesPerCore = 0;
	void ap_main(const u8 apicId);
	extern void longMode();
	void BSPInitialize(const BootInfo bootInfo)
	{
		constexpr u64 countPagesForInitAP = 5;
		const u64 apinit = reinterpret_cast<u64>(bootInfo.memoryMapEntries[0].address) - 0x1000 * countPagesForInitAP;
		console::printf("apinit: %llx\n", apinit);
		*(u64*)(apinit + 256 - 8) = reinterpret_cast<u64>(&longMode);
		console::printf("ap_main: %llx\n", *(u64*)(apinit + 256 - 8));
		memory::PageTable* apPLM4 = reinterpret_cast<memory::PageTable*>(apinit + 0x1000);
		set(apPLM4, 0, 0x1000);
		memory::PageTableManager apPageTableManager(apPLM4);
		for (size_t addr = 0x1000; addr < 0x200000; addr+=0x1000) //map first 2MiB 
		{
			apPageTableManager.fixedMapMemory(0x3000, (void*)(addr), (void*)(addr));
		}
		ap_stackNumberPagesPerCore = bootInfo.memoryMapEntries[0].numberOfPages;
		ap_stack = memory::allocator::allocBlocks((numlapic-1) * ap_stackNumberPagesPerCore);
		console::printf("ap_stackNumberPagesPerCore: %llx\n", ap_stackNumberPagesPerCore);
		console::printf("ap_stack: %llx\n", ap_stack);
		console::printf("lapic 0x280: %llx\n", APIC::lapics[0].read(APIC::LAPIC::Registers(0x280)));
		console::printf("lapic 0x310: %llx\n", APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)));
		console::printf("lapic 0x300: %llx\n", APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)));
		u64 lapic = reinterpret_cast<u64>(APIC::lapic);
		for (size_t i = 1; i < numlapic; i++)
		{
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x280), 0);
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x310), APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)) & 0x00ffffff | (APIC::lapics[i].id << 24));
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x300), APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & 0xfff00000 | 0x00C500);
			//((volatile u32*)(lapic + 0x310)) = (*((volatile u32*)(lapic + 0x310)) & 0x00ffffff) | (APIC::lapics[i].id << 24);
			//*((volatile u32*)(lapic + 0x300)) = (*((volatile u32*)(lapic + 0x300)) & 0xfff00000) | 0x00C500;
			do { _mm_pause(); } while (APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & (1 << 12));
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x310), APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)) & 0x00ffffff | (APIC::lapics[i].id << 24));
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x300), APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & 0xfff00000 | 0x008500);
			do { _mm_pause(); } while (APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & (1 << 12));
			PIT::sleep(10);
			console::printf("lapic 0x280: %llx\n", APIC::lapics[0].read(APIC::LAPIC::Registers(0x280)));
			console::printf("lapic 0x310: %llx\n", APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)));
			console::printf("lapic 0x300: %llx\n", APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)));
			// send STARTUP IPI (twice)
			for (size_t j = 0; j < 2; j++)
			{
				APIC::lapics[0].write(APIC::LAPIC::Registers(0x280), 0);
				APIC::lapics[0].write(APIC::LAPIC::Registers(0x310), APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)) & 0x00ffffff | (APIC::lapics[i].id << 24));
				APIC::lapics[0].write(APIC::LAPIC::Registers(0x300), APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & 0xfff0f800 | 0x000601);
				//*((volatile u32*)(lapic + 0x300)) = (*((volatile u32*)(lapic + 0x300)) & 0xfff0f800) | 0x000601;
				PIT::sleep(10);
				do { _mm_pause(); } while (APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & (1 << 12));
			}
		}
		bspdone = 1;
	}
	void ap_main(const u8 apicId)
	{
		while (true)
		{
			++APIC::aprunning;
			_mm_pause();
		}
	}
}