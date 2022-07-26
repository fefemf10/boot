#pragma once
#include "IDT.hpp"
#include "IO.hpp"
#include "TeletypeVideoBuffer.hpp"

enum Gate : u8
{
	INTERRUPT = 0xE,
	TRAP = 0xF
};
enum PIC : u8
{
	PIC1_C = 0x20,
	PIC1_D = 0x21,
	PIC2_C = 0xA0,
	PIC2_D = 0xA1,
	ICW4 = 0x01,
	ICW1 = 0x10,
	ICW_8086 = 0x01
};
enum KEYBOARD_PORT : u8
{
	DATA_PORT = 0x60,
	STATUS_PORT = 0x64
};
enum KEYBOARD_KEYS : u8
{
	ERROR,
	K_ESC,
	K_1,
	K_2,
	K_3,
	K_4,
	K_5,
	K_6,
	K_7,
	K_8,
	K_9,
	K_0,
	K_MINUS,
	K_PLUS,
	K_BACKSPACE,
	K_TAB,
	K_Q,
	K_W,
	K_E,
	K_R,
	K_T,
	K_Y,
	K_U,
	K_I,
	K_O,
	K_P,
	K_LEFTBRACKET,
	K_RIGHTBRACKET,
	K_ENTER,
	K_LEFTCTRL,
	K_A,
	K_S,
	K_D,
	K_F,
	K_G,
	K_H,
	K_J,
	K_K,
	K_L,
	K_SEMICOLON,
	K_RSQUO,
	K_TILDE,
	K_LEFTSHIFT,
	K_BACKSLASH,
	K_Z,
	K_X,
	K_C,
	K_V,
	K_B,
	K_N,
	K_M,
	K_LESS,
	K_GREATHER,
	K_SLASH,
	K_RIGHTSHIFT,
	K_PMUL,
	K_PRINTSCREEN = K_PMUL,
	K_LEFTALT,
	K_SPACEBAR,
	K_CASPLOCK,
	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_NUMLOCK,
	K_SCROLLLOCK,
	K_P7,
	K_P8,
	K_P9,
	K_PMINUS,
	K_P4,
	K_P5,
	K_P6,
	K_PPLUS,
	K_P1,
	K_P2,
	K_P3,
	K_P0,
	K_PCOMMA,
	K_ALTSYSREQ,
	K_NONMARKERALT1,
	K_NONMARKERALT2,
	K_F11,
	K_F12
};
static const char scancodes[] = {
	  0, 0, '1', '2',
	  '3', '4', '5', '6',
	  '7', '8', '9', '0',
	  '-', '=', 0, 0,

	  'q', 'w', 'e', 'r',
	  't', 'y', 'u', 'i',
	  'o', 'p', '[', ']',
	  0, 0, 'a', 's',

	  'd', 'f', 'g', 'h',
	  'j', 'k', 'l', ';',
	  '\'', '`', 0, '\\',
	  'z', 'x', 'c', 'v',

	  'b', 'n', 'm', ',',
	  '.', '/', 0, '*',
	  0, ' ', 0, 0,
	  0, 0, 0, 0,

	  0, 0, '!', '@',
	  '#', '$', '%', '^',
	  '&', '*', '(', ')',
	  '_', '+', '+', 0,

	  0, 0, 0, 0,
	  0, 0, 0, 0,
	  0, 0, '{', '}',
	  0, 0, 0, 0,

	  0, 0, 0, 0,
	  0, 0, 0, ':',
	  '"', '~', 0, '|',
	  0, 0, 0, 0,
};
static bool leftShiftPressed = false;
static bool rightShiftPressed = false;
static bool leftCtrlPressed = false;
static bool rightCtrlPressed = false;
static bool capsLock = false;
static bool numLock = false;
static unsigned char lastScancode{};

extern "C" IDT64 _idt[256];
extern "C" u64 isr33;
extern "C" u64 isr0;
extern "C" u64 isr1;
extern "C" u64 isr2;
extern "C" u64 isr3;
extern "C" u64 isr4;
extern "C" u64 isr5;
extern "C" u64 isr6;
extern "C" u64 isr7;
extern "C" u64 isr8;
extern "C" u64 isr9;
extern "C" u64 isr10;
extern "C" u64 isr11;
extern "C" u64 isr12;
extern "C" u64 isr13;
extern "C" u64 isr14;
extern "C" u64 isr15;
extern "C" u64 isr16;
extern "C" u64 isr17;
extern "C" u64 isr18;
extern "C" u64 isr19;
extern "C" u64 isr20;
extern "C" u64 isr21;
extern "C" u64 isr22;
extern "C" u64 isr23;
extern "C" u64 isr24;
extern "C" u64 isr25;
extern "C" u64 isr26;
extern "C" u64 isr27;
extern "C" u64 isr28;
extern "C" u64 isr29;
extern "C" u64 isr30;
extern "C" u64 isr31;
extern "C" void loadIDT();

void setMaskIRQ(u8 lineIRQ)
{
	u16 port;
	u8 value;
	if (lineIRQ < 8)
	{
		port = PIC1_D;
	}
	else
	{
		port = PIC2_D;
		lineIRQ -= 8;
	}
	value = inb(port) | (1 << lineIRQ);
	outb(value, port);
}

void clearMaskIRQ(u8 lineIRQ)
{
	u16 port;
	u8 value;
	if (lineIRQ < 8)
	{
		port = PIC1_D;
	}
	else
	{
		port = PIC2_D;
		lineIRQ -= 8;
	}
	value = inb(port) & ~(1 << lineIRQ);
	outb(value, port);
}

void setIDT(size_t index, u64& address)
{
	_idt[index].setOffset(&address);
	_idt[index].selector = 0x08;
	_idt[index].typeAttr = 0x80 | Gate::TRAP;
}

void initializeIDT()
{/*
	for (size_t i = 0; i < 32; i++)
	{
		_idt[i].setOffset(reinterpret_cast<u64*>(isr[i] + 0x8e00+2+16));
		_idt[i].selector = 0x08;
		_idt[i].typeAttr = 0x80 | Gate::TRAP;
	}*/
	setIDT(0, isr0);
	setIDT(1, isr1);
	setIDT(2, isr2);
	setIDT(3, isr3);
	setIDT(4, isr4);
	setIDT(5, isr5);
	setIDT(6, isr6);
	setIDT(7, isr7);
	setIDT(8, isr8);
	setIDT(9, isr9);
	setIDT(10, isr10);
	setIDT(11, isr11);
	setIDT(12, isr12);
	setIDT(13, isr13);
	setIDT(14, isr14);
	setIDT(15, isr15);
	setIDT(16, isr16);
	setIDT(17, isr17);
	setIDT(18, isr18);
	setIDT(19, isr19);
	setIDT(20, isr20);
	setIDT(21, isr21);
	setIDT(22, isr22);
	setIDT(23, isr23);
	setIDT(24, isr24);
	setIDT(25, isr25);
	setIDT(26, isr26);
	setIDT(27, isr27);
	setIDT(28, isr28);
	setIDT(29, isr29);
	setIDT(30, isr30);
	setIDT(31, isr31);
	_idt[0x21].setOffset(&isr33);
	_idt[0x21].selector = 0x08;
	_idt[0x21].typeAttr = 0x80 | Gate::INTERRUPT;

	u8 mask1 = inb(PIC1_D);
	u8 mask2 = inb(PIC2_D);
	//ICW1
	outb(ICW1 | ICW4, PIC1_C);
	outb(ICW1 | ICW4, PIC2_C);
	//ICW2
	outb(0x20, PIC1_D);//0x20 + 1 = 0x21 keyboard in IDT
	outb(0x28, PIC2_D);
	//ICW3
	outb(0x00, PIC1_D);
	outb(0x00, PIC2_D);
	//ICW4;
	outb(ICW_8086, PIC1_D);
	outb(ICW_8086, PIC2_D);
	//mask interrupt
	outb(mask1, PIC1_D);
	outb(mask2, PIC2_D);

	outb(0xFF, PIC1_D);
	outb(0xFF, PIC2_D);

	clearMaskIRQ(1);//enable IRQ from keyboard
	loadIDT();
}

void standartKeyboard(unsigned char scancode, unsigned char c)
{
	switch (lastScancode)
	{
	case 0xE0:

		switch (scancode)
		{
		case KEYBOARD_KEYS::K_P2:
			TeletypeVideoBuffer::setCursorPosition(TeletypeVideoBuffer::currentPos + static_cast<short>(TeletypeVideoBuffer::width));
			break;
		case KEYBOARD_KEYS::K_P8:
			TeletypeVideoBuffer::setCursorPosition(TeletypeVideoBuffer::currentPos - static_cast<short>(TeletypeVideoBuffer::width));
			break;
		case KEYBOARD_KEYS::K_P4:
			TeletypeVideoBuffer::setCursorPosition(TeletypeVideoBuffer::currentPos - 1);
			break;
		case KEYBOARD_KEYS::K_P6:
			TeletypeVideoBuffer::setCursorPosition(TeletypeVideoBuffer::currentPos + 1);
			break;
		case KEYBOARD_KEYS::K_ENTER://keypad enter
			TeletypeVideoBuffer::puts("\n\r");
			break;
		case KEYBOARD_KEYS::K_LEFTCTRL://right ctrl
			rightCtrlPressed = true;
			break;
		case KEYBOARD_KEYS::K_LEFTCTRL | 0x80://right ctrl released
			rightCtrlPressed = false;
			break;
		case KEYBOARD_KEYS::K_SLASH:
			TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_SLASH]);
			break;
		}
		break;
	default:
		if (c && scancode < 0x3A)
		{
			bool additional = scancode == KEYBOARD_KEYS::K_TILDE || scancode == KEYBOARD_KEYS::K_SEMICOLON || scancode == KEYBOARD_KEYS::K_RSQUO || scancode == KEYBOARD_KEYS::K_LEFTBRACKET || scancode == KEYBOARD_KEYS::K_RIGHTBRACKET || scancode == KEYBOARD_KEYS::K_BACKSLASH || scancode >= KEYBOARD_KEYS::K_1 && scancode <= KEYBOARD_KEYS::K_PLUS;
			if ((leftShiftPressed || rightShiftPressed) && additional)
			{
				TeletypeVideoBuffer::putc(scancodes[scancode + 64]);
			}
			else
			{
				switch (((leftShiftPressed | rightShiftPressed) ^ capsLock) && !additional)
				{
				case true:
					TeletypeVideoBuffer::putc(c - 32);
					break;
				case false:
					TeletypeVideoBuffer::putc(c);
					break;
				}
			}
		}
		else
		{
			switch (scancode)
			{
			case KEYBOARD_KEYS::K_BACKSPACE://backspace
				TeletypeVideoBuffer::setCursorPosition(TeletypeVideoBuffer::currentPos - 1);
				TeletypeVideoBuffer::putc(' ');
				TeletypeVideoBuffer::setCursorPosition(TeletypeVideoBuffer::currentPos - 1);
				break;
			case KEYBOARD_KEYS::K_LEFTSHIFT://left shift
				leftShiftPressed = true;
				break;
			case KEYBOARD_KEYS::K_LEFTSHIFT | 0x80://left shift release
				leftShiftPressed = false;
				break;
			case KEYBOARD_KEYS::K_RIGHTSHIFT://right shift
				rightShiftPressed = true;
				break;
			case KEYBOARD_KEYS::K_RIGHTSHIFT | 0x80://right shift release
				rightShiftPressed = false;
				break;
			case KEYBOARD_KEYS::K_LEFTCTRL://right ctrl
				rightCtrlPressed = true;
				break;
			case KEYBOARD_KEYS::K_LEFTCTRL | 0x80://right ctrl release
				rightCtrlPressed = false;
				break;
			case KEYBOARD_KEYS::K_ENTER://enter
				TeletypeVideoBuffer::puts("\n\r");
				break;
			case KEYBOARD_KEYS::K_CASPLOCK://capslock
				capsLock ^= lastScancode != scancode;
				break;
			case KEYBOARD_KEYS::K_NUMLOCK://numlock
				numLock ^= lastScancode != scancode;
				break;
			case KEYBOARD_KEYS::K_PPLUS:
				TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_PPLUS]);
				break;
			case KEYBOARD_KEYS::K_PMINUS:
				TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_MINUS]);
				break;
			case KEYBOARD_KEYS::K_PMUL:
				TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_PMUL]);
				break;
			case KEYBOARD_KEYS::K_ESC://reboot 
				reboot();
				break;
			}
			if (numLock)
			{
				switch (scancode)
				{
				case KEYBOARD_KEYS::K_P0:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_0]);
					break;
				case KEYBOARD_KEYS::K_P1:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_1]);
					break;
				case KEYBOARD_KEYS::K_P2:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_2]);
					break;
				case KEYBOARD_KEYS::K_P3:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_3]);
					break;
				case KEYBOARD_KEYS::K_P4:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_4]);
					break;
				case KEYBOARD_KEYS::K_P5:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_5]);
					break;
				case KEYBOARD_KEYS::K_P6:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_6]);
					break;
				case KEYBOARD_KEYS::K_P7:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_7]);
					break;
				case KEYBOARD_KEYS::K_P8:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_8]);
					break;
				case KEYBOARD_KEYS::K_P9:
					TeletypeVideoBuffer::putc(scancodes[KEYBOARD_KEYS::K_9]);
					break;
				}
			}
		}
		break;
	}
	//TeletypeVideoBuffer::puts(TeletypeVideoBuffer::hexToString(scancode));
	lastScancode = scancode;
}

extern "C" void isr33_handler()
{
	unsigned char status;
	unsigned char keycode;
	
	status = inb(KEYBOARD_PORT::STATUS_PORT);
	if (status & 0x01)
	{
		keycode = inb(KEYBOARD_PORT::DATA_PORT);
		standartKeyboard(keycode, scancodes[keycode]);
	}
	outb(0x20, PIC1_C);//PIC1 обработали прерывание EOI
}

void IDT64::setOffset(u64* offset)
{
	offsetLow = static_cast<u16>(reinterpret_cast<u64>(offset) & 0x000000000000ffff);
	offsetMid = static_cast<u16>((reinterpret_cast<u64>(offset) & 0x00000000ffff0000) >> 16u);
	offsetHigh = static_cast<u32>((reinterpret_cast<u64>(offset) & 0xffffffff00000000) >> 32u);
}

u64* IDT64::getOffset()
{
	u64 offset{};
	offset |= static_cast<u64>(offsetLow);
	offset |= static_cast<u64>(offsetMid) << 16u;
	offset |= static_cast<u64>(offsetHigh) << 32u;
	return reinterpret_cast<u64*>(offset);
}