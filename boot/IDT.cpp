#pragma once
#include "stdtype.hpp"
#include "IO.hpp"
#include "TeletypeVideoBuffer.hpp"


enum PIC : unsigned char
{
	PIC1_C = 0x20,
	PIC1_D = 0x21,
	PIC2_C = 0xA0,
	PIC2_D = 0xA1,
};
enum KEYBOARD_PORT : unsigned char
{
	DATA_PORT = 0x60,
	STATUS_PORT = 0x64
};
enum KEYBOARD_KEYS : unsigned char
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
struct IDT64
{
	u16 offset_low;
	u16 selector;
	u8 ist;
	u8 type_attr;
	u16 offset_mid;
	u32 offset_high;
	u32 zero;
};
extern "C" IDT64 _idt[256];
extern "C" u64 isr1;
extern "C" void loadIDT();

void initializeIDT()
{
	for (u64 i = 0x21; i < 0x22; i++)//0x21 keyboard
	{
		_idt[i].offset_low = (u16)(((u64)&isr1 & 0x000000000000ffff));
		_idt[i].selector = 0x08;
		_idt[i].ist = 0x0;
		_idt[i].type_attr = 0x8e;
		_idt[i].offset_mid = (u16)(((u64)&isr1 & 0x00000000ffff0000) >> 16u);
		_idt[i].offset_high = (u32)(((u64)&isr1 & 0xffffffff00000000) >> 32u);
		_idt[i].zero = 0;
	}
	unsigned char a1, a2;

	a1 = inb(PIC1_D);
	a2 = inb(PIC2_D);
	//ICW1
	outb(0x11, PIC1_C);
	outb(0x11, PIC2_C);
	//ICW2
	outb(0x20, PIC1_D);//0x20 + 1 = 0x21 keyboard in IDT
	outb(0x28, PIC2_D);
	//ICW3
	outb(0x00, PIC1_D);
	outb(0x00, PIC2_D);
	//ICW4
	outb(0x01, PIC1_D);
	outb(0x01, PIC2_D);
	//mask interrupts
	outb(a1, PIC1_D);
	outb(a2, PIC2_D);

	outb(0xfd, PIC1_D);
	outb(0xff, PIC2_D);
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
				switch ((leftShiftPressed | rightShiftPressed) ^ capsLock && !additional)
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

extern "C" void isr1_handler()
{
	unsigned char status;
	unsigned char keycode;
	outb(0x20, 0x20);//PIC1 обработали прерывание EOI
	outb(0x20, 0xa0);//PIC2 обработали прерывание EOI
	status = inb(KEYBOARD_PORT::STATUS_PORT);
	if (status & 0x01)
	{
		keycode = inb(KEYBOARD_PORT::DATA_PORT);
		standartKeyboard(keycode, scancodes[keycode]);
	}
}