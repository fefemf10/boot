export module IRQ;
import types;
import cpuio;
import keyboard;
import APIC;
import IOAPIC;
import PIT;
import intrinsic1;
import APICTimer;
import console;
import serial;
import sl.string_view;
import RTC;
import IRQ.Number;
import HPETTimer;
export namespace IRQ
{
	extern "C" void irqHandler(const cpuio::regs& regs)
	{
		const size_t code = regs.interruptCode - 0x20;
		u8 status, keycode;
		switch (code)
		{
		case 0:
			PIT::tick();
			break;
		case 1:
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
		case 3:
			HPETTimer::tick();
			break;
		case 8:
			RTC::tick();
			__outbyte(0x70, 0x0C);
			__inbyte(0x71);
			break;
		}
		APIC::lapics[0].write(APIC::LAPIC::Registers::EOI, 0);
	}
	
	void initialize()
	{
		APIC::IOAPIC::REDTBLEntry RTCInterrupt{};
		RTCInterrupt.vector = Number::RTC;
		RTCInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		RTCInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		RTCInterrupt.pinPolarity = 0;
		RTCInterrupt.destination = APIC::lapics[0].id;
		RTCInterrupt.mask = 0;
		APIC::ioapics[0].writeREDTBL(8, RTCInterrupt);
		APIC::IOAPIC::REDTBLEntry PITInterrupt{};
		PITInterrupt.vector = Number::PIT;
		PITInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		PITInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		PITInterrupt.pinPolarity = 0;
		PITInterrupt.destination = APIC::lapics[0].id;
		PITInterrupt.mask = 0;
		APIC::ioapics[0].writeREDTBL(2, PITInterrupt);
		APIC::IOAPIC::REDTBLEntry keyboardInterrupt{};
		keyboardInterrupt.vector = Number::KEYBOARD;
		keyboardInterrupt.deliveryMode = APIC::IOAPIC::DeliveryMode::EDGE;
		keyboardInterrupt.destinationMode = APIC::IOAPIC::DesctinationMode::PHYSICAL;
		keyboardInterrupt.pinPolarity = 0;
		keyboardInterrupt.destination = APIC::lapics[0].id;
		keyboardInterrupt.mask = 0;
		APIC::ioapics[0].writeREDTBL(1, keyboardInterrupt);
	}
}