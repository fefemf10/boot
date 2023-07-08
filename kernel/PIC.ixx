export module PIC;
import cpuio;
import types;
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
		cpuio::outb(ICW1 | ICW4, PIC1_C);
		cpuio::outb(ICW1 | ICW4, PIC2_C);
		//ICW2
		cpuio::outb(0x20, PIC1_D);
		cpuio::outb(0x28, PIC2_D);
		//ICW3
		cpuio::outb(0x04, PIC1_D);
		cpuio::outb(0x02, PIC2_D);
		//ICW4
		cpuio::outb(ICW_8086, PIC1_D);
		cpuio::outb(ICW_8086, PIC2_D);
		//Masking all interrupt
		cpuio::outb(0xFF, PIC1_D);
		cpuio::outb(0xFF, PIC2_D);
	}
	void deinitialize()
	{
		//Masking all interrupt
		cpuio::outb(0xFF, PIC1_D);
		cpuio::outb(0xFF, PIC2_D);
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
		data = cpuio::inb(port);
		if (value > 0)
		{
			data |= (1 << index);
		}
		else
		{
			data &= ~(1 << index);
		}
		cpuio::outb(data, port);
	}
	void eioPrimary()
	{
		cpuio::outb(0x20, PIC::PIC1_C);
	}
	void eioSecondary()
	{
		cpuio::outb(0x20, PIC::PIC2_C);
		cpuio::outb(0x20, PIC::PIC1_C);
	}
}