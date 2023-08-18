export module APICTimer;
import types;
import intrinsic0;
export namespace APICTimer
{
	double timeSinceBoot{ 0.0 };
	u64 ticks = 0;
	u64 frequency = 0;
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
	void tick()
	{
		++ticks;
		timeSinceBoot += 1.0 / frequency;
	}
}