export module IRQ.Number;
import types;
export namespace IRQ::Number
{
	const u8 PIT = 0x20;
	const u8 KEYBOARD = 0x21;
	const u8 APICTIMER = 0x22;
	const u8 RTC = 0x28;
	const u8 HPET = 0x23;
}