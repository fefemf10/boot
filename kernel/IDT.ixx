export module IDT;
import types;
import memory.utils;
import IRQ.Number;
export namespace IDT
{
	enum Gate : u8
	{
		INTERRUPT = 0xE,
		TRAP = 0xF
	};
#pragma pack(1)
	struct IDTR
	{
		u16 size;
		u64 offset;
	};
#pragma pack()
	struct IDT
	{
		u16 offsetLow;
		u16 selector;
		u8 ist;
		u8 typeAttr;
		u16 offsetMid;
		u32 offsetHigh;
		u32 zero;
		void setOffset(void (*function)())
		{
			offsetLow = static_cast<u16>(reinterpret_cast<u64>(function) & 0x000000000000ffff);
			offsetMid = static_cast<u16>((reinterpret_cast<u64>(function) & 0x00000000ffff0000) >> 16u);
			offsetHigh = static_cast<u32>((reinterpret_cast<u64>(function) & 0xffffffff00000000) >> 32u);
		}
		u64* getOffset()
		{
			u64 offset{};
			offset |= static_cast<u64>(offsetLow);
			offset |= static_cast<u64>(offsetMid) << 16u;
			offset |= static_cast<u64>(offsetHigh) << 32u;
			return reinterpret_cast<u64*>(offset);
		}
	};

	extern "C" void isr0();
	extern "C" void isr1();
	extern "C" void isr2();
	extern "C" void isr3();
	extern "C" void isr4();
	extern "C" void isr5();
	extern "C" void isr6();
	extern "C" void isr7();
	extern "C" void isr8();
	extern "C" void isr9();
	extern "C" void isr10();
	extern "C" void isr11();
	extern "C" void isr12();
	extern "C" void isr13();
	extern "C" void isr14();
	extern "C" void isr15();
	extern "C" void isr16();
	extern "C" void isr17();
	extern "C" void isr18();
	extern "C" void isr19();
	extern "C" void isr20();
	extern "C" void isr21();
	extern "C" void isr22();
	extern "C" void isr23();
	extern "C" void isr24();
	extern "C" void isr25();
	extern "C" void isr26();
	extern "C" void isr27();
	extern "C" void isr28();
	extern "C" void isr29();
	extern "C" void isr30();
	extern "C" void isr31();

	extern "C" void irq0();
	extern "C" void irq1();
	extern "C" void irq2();
	extern "C" void irq3();
	extern "C" void irq4();
	extern "C" void irq5();
	extern "C" void irq6();
	extern "C" void irq7();
	extern "C" void irq8();
	extern "C" void irq9();
	extern "C" void irq10();
	extern "C" void irq11();
	extern "C" void irq12();
	extern "C" void irq13();
	extern "C" void irq14();
	extern "C" void irq15();

	IDT idt[256]{};
	IDTR idtr{ .size = 0x0FFF, .offset = reinterpret_cast<u64>(idt) };
	void set(size_t index, void(*function)())
	{
		idt[index].setOffset(function);
		idt[index].selector = 0x08;
		idt[index].typeAttr = 0x80 | Gate::INTERRUPT;
	}
	void initialize()
	{
		memory::set(idt, 0x00, sizeof(idt));

		set(0, isr0);
		set(1, isr1);
		set(2, isr2);
		set(3, isr3);
		set(4, isr4);
		set(5, isr5);
		set(6, isr6);
		set(7, isr7);
		set(8, isr8);
		set(9, isr9);
		set(10, isr10);
		set(11, isr11);
		set(12, isr12);
		set(13, isr13);
		set(14, isr14);
		set(15, isr15);
		set(16, isr16);
		set(17, isr17);
		set(18, isr18);
		set(19, isr19);
		set(20, isr20);
		set(21, isr21);
		set(22, isr22);
		set(23, isr23);
		set(24, isr24);
		set(25, isr25);
		set(26, isr26);
		set(27, isr27);
		set(28, isr28);
		set(29, isr29);
		set(30, isr30);
		set(31, isr31);

		set(IRQ::Number::PIT, irq0);
		set(IRQ::Number::KEYBOARD, irq1);
		set(IRQ::Number::APICTIMER, irq2);
		set(IRQ::Number::HPET, irq3);
		//IDT::set(34, irq2);
		//IDT::set(35, irq3);
		//IDT::set(36, irq4);
		//IDT::set(37, irq5);
		//IDT::set(38, irq6);
		//IDT::set(39, irq7);
		set(IRQ::Number::RTC, irq8);
		//IDT::set(41, irq9);
		//IDT::set(42, irq10);
		//IDT::set(43, irq11);
		//IDT::set(44, irq12);
		//IDT::set(45, irq13);
		//IDT::set(46, irq14);
		//IDT::set(47, irq15);
	}
}