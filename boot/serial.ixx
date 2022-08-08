export module serial;
import types;
import cpuio;
export namespace serial
{
	constexpr u16 PORT = 0x3f8;
	void initialize()
	{
		cpuio::outb(0x00, PORT + 1);
		cpuio::outb(0x80, PORT + 3);
		cpuio::outb(0x01, PORT + 0);
		cpuio::outb(0x00, PORT + 1);
		cpuio::outb(0x03, PORT + 3);
		cpuio::outb(0xC7, PORT + 2);
		cpuio::outb(0x0B, PORT + 4);
		cpuio::outb(0x1E, PORT + 4);
		cpuio::outb(0xAE, PORT + 0);
		cpuio::outb(0x0F, PORT + 4);
	}
	u8 read()
	{
		while ((cpuio::inb(PORT + 5) & 1) == 0);
		return cpuio::inb(PORT);
	}
	void write(u8 data)
	{
		while ((cpuio::inb(PORT + 5) & 0x20) == 0);
		cpuio::outb(data, PORT);
	}
}