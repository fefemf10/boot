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
	extern void (*isrHandlers[32])();
	extern void (*irqHandlers[16])();

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
		for (size_t i = 0; i < 32; i++)
			set(i, isrHandlers[i]);

		set(IRQ::Number::PIT, irqHandlers[0]);
		set(IRQ::Number::KEYBOARD, irqHandlers[1]);
		set(IRQ::Number::APICTIMER, irqHandlers[2]);
		set(IRQ::Number::HPET, irqHandlers[3]);
		//IDT::set(34, irq2);
		//IDT::set(35, irq3);
		//IDT::set(36, irq4);
		//IDT::set(37, irq5);
		//IDT::set(38, irq6);
		//IDT::set(39, irq7);
		set(IRQ::Number::RTC, irqHandlers[8]);
		//IDT::set(41, irq9);
		//IDT::set(42, irq10);
		//IDT::set(43, irq11);
		//IDT::set(44, irq12);
		//IDT::set(45, irq13);
		//IDT::set(46, irq14);
		//IDT::set(47, irq15);
	}
}