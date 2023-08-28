export module SMP;
import types;
import APIC;
import LAPIC;
import intrinsic0;
import intrinsic1;
import BootInfo;
import PIT;
import memory.PageTable;
import memory.PageTableManager;
import memory.allocator;
import memory.utils;

export namespace APIC
{
	void BSPInitialize(const BootInfo bootInfo)
	{
		constexpr u64 countPagesForInitAP = 5;
		const u64 apinit = reinterpret_cast<u64>(bootInfo.memoryMapEntries[0].address) - 0x1000 * countPagesForInitAP;
		*(u64*)(apinit + 256 - 8) = reinterpret_cast<u64>(&longMode);
		memory::PageTable* apPLM4 = reinterpret_cast<memory::PageTable*>(apinit + 0x1000);
		memory::set(apPLM4, 0, 0x1000);
		memory::PageTableManager apPageTableManager(apPLM4);
		for (size_t addr = 0x1000; addr < 0x200000; addr += 0x1000) //map first 2MiB 
		{
			apPageTableManager.fixedMapMemory(0x3000, (void*)(addr), (void*)(addr));
		}
		ap_stackNumberPagesPerCore = bootInfo.memoryMapEntries[0].numberOfPages;
		ap_stack = memory::allocator::allocBlocks((numlapic - 1) * ap_stackNumberPagesPerCore);
		u64 lapic = reinterpret_cast<u64>(APIC::lapic);
		for (size_t i = 1; i < numlapic; i++)
		{
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x280), 0);
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x310), APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)) & 0x00ffffff | (APIC::lapics[i].id << 24));
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x300), APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & 0xfff00000 | 0x00C500);
			do { _mm_pause(); } while (APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & (1 << 12));
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x310), APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)) & 0x00ffffff | (APIC::lapics[i].id << 24));
			APIC::lapics[0].write(APIC::LAPIC::Registers(0x300), APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & 0xfff00000 | 0x008500);
			do { _mm_pause(); } while (APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & (1 << 12));
			PIT::sleep(10);
			for (size_t j = 0; j < 2; j++)
			{
				APIC::lapics[0].write(APIC::LAPIC::Registers(0x280), 0);
				APIC::lapics[0].write(APIC::LAPIC::Registers(0x310), APIC::lapics[0].read(APIC::LAPIC::Registers(0x310)) & 0x00ffffff | (APIC::lapics[i].id << 24));
				APIC::lapics[0].write(APIC::LAPIC::Registers(0x300), APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & 0xfff0f800 | 0x000601);
				PIT::sleep(10);
				do { _mm_pause(); } while (APIC::lapics[0].read(APIC::LAPIC::Registers(0x300)) & (1 << 12));
			}
		}
		bspdone = 1;
	}
	void ap_main(const u8 apicId)
	{
		while (true) __halt();
	}
}