export module PIT;
import types;
import cpuio;
export namespace PIT
{
	double timeSinceBoot{};
	const u64 baseFrequency = 1193182;
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
	void setDivisor(u16 divisor)
	{
		if (divisor < 100) PIT::divisor = 100;
		cpuio::outb(static_cast<u8>(divisor & 0x00ff), 0x40);
		cpuio::iowait();
		cpuio::outb(static_cast<u8>((divisor & 0xff00) >> 8), 0x40);
	}
	u64 getFrequency()
	{
		return baseFrequency / divisor;
	}
	void setFrequency(u64 frequency)
	{
		setDivisor(baseFrequency / frequency);
	}
	void tick()
	{
		timeSinceBoot += 1.0 / getFrequency();
	}
}