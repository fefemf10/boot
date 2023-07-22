export module PIT;
import types;
import cpuio;
import intrinsic0;
import intrinsic1;
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
			_mm_pause();
		}
	}
	void sleep(u64 millisec)
	{
		sleepd(millisec / 1000.0);
	}
	void setDivisor(i32 divisor)
	{
		PIT::divisor = divisor;
		__outbyte(0x43, 0x34);
		__outbyte(0x40, static_cast<u8>(divisor & 0x00ff));
		__outbyte(0x40, static_cast<u8>((divisor & 0xff00) >> 8));
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