export module PIT;
import types;
import cpuio;
export namespace PIT
{
	double timeSinceBoot{0.0};
	const i32 baseFrequency = 1193182;
	u64 ticks = 0;
	i32 frequency = 0;
	u16 divisor = 65535;
	void sleepd(double seconds)
	{
		double start = timeSinceBoot;
		while (timeSinceBoot < start + seconds)
		{
			cpuio::halt();
		}
	}
	void sleep(u64 millisec)
	{
		sleepd(millisec / 1000.0);
	}
	void setDivisor(i32 divisor)
	{
		PIT::divisor = divisor;
		cpuio::outb(0x34, 0x43);
		cpuio::outb(static_cast<u8>(divisor & 0x00ff), 0x40);
		cpuio::outb(static_cast<u8>((divisor & 0xff00) >> 8), 0x40);
	}
	double getFrequency()
	{
		return baseFrequency / divisor;
	}
	void setFrequency(i32 frequency)
	{
		i32 f = baseFrequency / frequency;
		PIT::frequency = frequency;
		if (f < 1)
		{
			f = 1;
		}
		else if (f > 65535)
		{
			f = 65535;
		}
		setDivisor(f);
	}
	void tick()
	{
		++ticks;
		timeSinceBoot += 1.0 / getFrequency();
	}
}