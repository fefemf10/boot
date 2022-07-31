export module keyboard;
import types;
import teletype;
import cpuio;
export namespace keyboard
{
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
	constexpr char scancodes[] = {
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
	bool leftShiftPressed = false;
	bool rightShiftPressed = false;
	bool leftCtrlPressed = false;
	bool rightCtrlPressed = false;
	bool capsLock = false;
	bool numLock = false;
	u8 lastScancode{};
	void standartKeyboard(unsigned char scancode, unsigned char c)
	{
		switch (lastScancode)
		{
		case 0xE0:

			switch (scancode)
			{
			case KEYBOARD_KEYS::K_P2:
				teletype::setCursorPosition(teletype::currentPos + static_cast<short>(teletype::width));
				break;
			case KEYBOARD_KEYS::K_P8:
				teletype::setCursorPosition(teletype::currentPos - static_cast<short>(teletype::width));
				break;
			case KEYBOARD_KEYS::K_P4:
				teletype::setCursorPosition(teletype::currentPos - 1);
				break;
			case KEYBOARD_KEYS::K_P6:
				teletype::setCursorPosition(teletype::currentPos + 1);
				break;
			case KEYBOARD_KEYS::K_ENTER://keypad enter
				teletype::puts("\n\r");
				break;
			case KEYBOARD_KEYS::K_LEFTCTRL://right ctrl
				rightCtrlPressed = true;
				break;
			case KEYBOARD_KEYS::K_LEFTCTRL | 0x80://right ctrl released
				rightCtrlPressed = false;
				break;
			case KEYBOARD_KEYS::K_SLASH:
				teletype::putc(scancodes[KEYBOARD_KEYS::K_SLASH]);
				break;
			}
			break;
		default:
			if (c && scancode < 0x3A)
			{
				bool additional = scancode == KEYBOARD_KEYS::K_TILDE || scancode == KEYBOARD_KEYS::K_SEMICOLON || scancode == KEYBOARD_KEYS::K_RSQUO || scancode == KEYBOARD_KEYS::K_LEFTBRACKET || scancode == KEYBOARD_KEYS::K_RIGHTBRACKET || scancode == KEYBOARD_KEYS::K_BACKSLASH || scancode >= KEYBOARD_KEYS::K_1 && scancode <= KEYBOARD_KEYS::K_PLUS;
				if ((leftShiftPressed || rightShiftPressed) && additional)
				{
					teletype::putc(scancodes[scancode + 64]);
				}
				else
				{
					switch (((leftShiftPressed | rightShiftPressed) ^ capsLock) && !additional)
					{
					case true:
						teletype::putc(static_cast<i8>(c - 32));
						break;
					case false:
						teletype::putc(c);
						break;
					}
				}
			}
			else
			{
				switch (scancode)
				{
				case KEYBOARD_KEYS::K_BACKSPACE://backspace
					teletype::setCursorPosition(teletype::currentPos - 1);
					teletype::putc(' ');
					teletype::setCursorPosition(teletype::currentPos - 1);
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
					teletype::puts("\n\r");
					break;
				case KEYBOARD_KEYS::K_CASPLOCK://capslock
					capsLock ^= lastScancode != scancode;
					break;
				case KEYBOARD_KEYS::K_NUMLOCK://numlock
					numLock ^= lastScancode != scancode;
					break;
				case KEYBOARD_KEYS::K_PPLUS:
					teletype::putc(scancodes[KEYBOARD_KEYS::K_PPLUS]);
					break;
				case KEYBOARD_KEYS::K_PMINUS:
					teletype::putc(scancodes[KEYBOARD_KEYS::K_MINUS]);
					break;
				case KEYBOARD_KEYS::K_PMUL:
					teletype::putc(scancodes[KEYBOARD_KEYS::K_PMUL]);
					break;
				case KEYBOARD_KEYS::K_ESC://reboot 
					cpuio::reboot();
					break;
				}
				if (numLock)
				{
					switch (scancode)
					{
					case KEYBOARD_KEYS::K_P0:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_0]);
						break;
					case KEYBOARD_KEYS::K_P1:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_1]);
						break;
					case KEYBOARD_KEYS::K_P2:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_2]);
						break;
					case KEYBOARD_KEYS::K_P3:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_3]);
						break;
					case KEYBOARD_KEYS::K_P4:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_4]);
						break;
					case KEYBOARD_KEYS::K_P5:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_5]);
						break;
					case KEYBOARD_KEYS::K_P6:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_6]);
						break;
					case KEYBOARD_KEYS::K_P7:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_7]);
						break;
					case KEYBOARD_KEYS::K_P8:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_8]);
						break;
					case KEYBOARD_KEYS::K_P9:
						teletype::putc(scancodes[KEYBOARD_KEYS::K_9]);
						break;
					}
				}
			}
			break;
		}
		lastScancode = scancode;
	}
}