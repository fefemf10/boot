export module IRQ;
import types;
import IDT;
import cpuio;
import keyboard;
import APIC;
import PIT;
import intrinsic1;
import APICTimer;
export namespace IRQ
{
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
	extern "C" void irqHandler(const cpuio::regs& regs)
	{
		size_t code = regs.interruptCode - 0x20;
		switch (code)
		{
		case 0:
			PIT::tick();
			break;
		case 1:
			u8 status;
			u8 keycode;

			status = __inbyte(keyboard::KEYBOARD_PORT::STATUS_PORT);
			if (status & 0x01)
			{
				keycode = __inbyte(keyboard::KEYBOARD_PORT::DATA_PORT);
				keyboard::standartKeyboard(keycode, keyboard::scancodes[keycode]);
			}
			break;
		case 2:
			APICTimer::tick();
			break;
		}
		APIC::lapics[0].write(APIC::LAPIC::Registers::EOI, 0);
	}
	void initialize()
	{
		IDT::set(32, irq0);
		IDT::set(33, irq1);
		IDT::set(34, irq2);
		//IDT::set(34, irq2);
		//IDT::set(35, irq3);
		//IDT::set(36, irq4);
		//IDT::set(37, irq5);
		//IDT::set(38, irq6);
		//IDT::set(39, irq7);
		//IDT::set(40, irq8);
		//IDT::set(41, irq9);
		//IDT::set(42, irq10);
		//IDT::set(43, irq11);
		//IDT::set(44, irq12);
		//IDT::set(45, irq13);
		//IDT::set(46, irq14);
		//IDT::set(47, irq15);
	}
}