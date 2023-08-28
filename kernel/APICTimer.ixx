export module APICTimer;
import types;
import intrinsic0;
export namespace APICTimer
{
	enum
	{
		DISABLE = 0x10000,
		PERIODIC = 0x20000,
	};
	volatile f64 timeSinceBoot{ 0.0 };
	volatile u64 ticks = 0;
	f64 frequency = 0;
	void initialize()
	{
		timeSinceBoot = 0.0;
		ticks = 0;
		frequency = 0.0;
	}
	void sleepd(f64 seconds)
	{
		volatile f64 start = timeSinceBoot;
		while (timeSinceBoot < start + seconds)
		{
			_mm_pause();
		}
	}
	void sleep(u64 millisec)
	{
		sleepd(millisec / 1000.0);
	}
	void tick()
	{
		++ticks;
		timeSinceBoot += 1.0 / frequency;
	}
}