export module PIC;
import cpuio;
import types;
import intrinsic1;
export namespace PIC
{
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
	void initialize()
	{
		//ICW1
		__outbyte(PIC1_C, ICW1 | ICW4);
		__outbyte(PIC2_C, ICW1 | ICW4);
		//ICW2
		__outbyte(PIC1_D, 0x20);
		__outbyte(PIC2_D, 0x28);
		//ICW3
		__outbyte(PIC1_D, 0x04);
		__outbyte(PIC2_D, 0x02);
		//ICW4
		__outbyte(PIC1_D, ICW_8086);
		__outbyte(PIC2_D, ICW_8086);
		//Masking all interrupt
		__outbyte(PIC1_D, 0xFF);
		__outbyte(PIC2_D, 0xFF);
	}
	void deinitialize()
	{
		//Masking all interrupt
		__outbyte(PIC1_D, 0xFF);
		__outbyte(PIC2_D, 0xFF);
	}
	void setMask(u8 index, u8 value)
	{
		u16 port;
		u8 data;
		if (index < 8)
		{
			port = PIC1_D;
		}
		else
		{
			port = PIC2_D;
			index -= 8;
		}
		data = __inbyte(port);
		if (value > 0)
		{
			data |= (1 << index);
		}
		else
		{
			data &= ~(1 << index);
		}
		__outbyte(port, data);
	}
	void eioPrimary()
	{
		__outbyte(PIC1_C, 0x20);
	}
	void eioSecondary()
	{
		__outbyte(PIC2_C, 0x20);
		__outbyte(PIC1_C, 0x20);
	}
}