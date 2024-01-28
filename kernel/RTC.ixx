export module RTC;
import types;
import intrinsic0;
import intrinsic1;
import cpuio;
export namespace RTC
{
	enum Registers
	{
		SECONDS = 0x00,
		MINUTES = 0x02,
		HOURS = 0x04,
		WEEKDAY = 0x06,
		DAYOFMONTH = 0x07,
		MONTH = 0x08,
		YEAR = 0x09,
		CENTURY = 0x32,
		A = 0x0A,
		B = 0x0B,
		C = 0x0C
	};
}
namespace RTC
{
	constexpr auto CURRENT_YEAR = 2024;
	int get_update_in_progress_flag()
	{
		__outbyte(0x70, 0x0A);
		return (__inbyte(0x71) & 0x80);
	}

	unsigned char readRegister(Registers reg)
	{
		__outbyte(0x70, reg);
		return __inbyte(0x71);
	}
}
export namespace RTC
{
	u8 centuryRegister;
	u8 century;
	u8 seconds;
	u8 minutes;
	u8 hours;
	u8 day;
	u8 month;
	u32 year;
	u8 rate;
	f64 frequency;
	volatile u64 ticks;
	volatile f64 timeSinceBoot;
	void initialize()
	{
		ticks = 0;
		timeSinceBoot = 0.0;
		rate = 3;
		frequency = 32768 >> (rate - 1);
	}
	void enable()
	{
		bool isEnabledInterrupt = cpuio::isEnabledInterrupt();
		_disable();
		__outbyte(0x70, 0x8B);
		char prev = __inbyte(0x71);
		__outbyte(0x70, 0x8B);
		__outbyte(0x71, prev | 0x40);
		if(isEnabledInterrupt) _enable();
	}
	void changeRate()
	{
		bool isEnabledInterrupt = cpuio::isEnabledInterrupt();
		_disable();
		__outbyte(0x70, 0x8A);
		char prev = __inbyte(0x71);
		__outbyte(0x70, 0x8A);
		__outbyte(0x71, (prev & 0xF0) | rate);
		if (isEnabledInterrupt) _enable();
	}
	void tick()
	{
		++ticks;
		timeSinceBoot += 1.0 / frequency;
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
	void read()
	{
		u8 lastCentury;
		u8 lastSeconds;
		u8 lastMinutes;
		u8 lastHours;
		u8 lastDay;
		u8 lastMonth;
		u32 lastYear;
		u8 registerB;
		while (get_update_in_progress_flag()) _mm_pause();
		seconds = readRegister(Registers::SECONDS);
		minutes = readRegister(Registers::MINUTES);
		hours = readRegister(Registers::HOURS);
		day = readRegister(Registers::DAYOFMONTH);
		month = readRegister(Registers::MONTH);
		year = readRegister(Registers::YEAR);
		if (centuryRegister)
			century = readRegister(static_cast<RTC::Registers>(centuryRegister));
		do
		{
			lastSeconds = seconds;
			lastMinutes = minutes;
			lastHours = hours;
			lastDay = day;
			lastMonth = month;
			lastYear = year;
			lastCentury = century;

			while (get_update_in_progress_flag()) _mm_pause();
			seconds = readRegister(Registers::SECONDS);
			minutes = readRegister(Registers::MINUTES);
			hours = readRegister(Registers::HOURS);
			day = readRegister(Registers::DAYOFMONTH);
			month = readRegister(Registers::MONTH);
			year = readRegister(Registers::YEAR);
			if (centuryRegister)
				century = readRegister(static_cast<RTC::Registers>(centuryRegister));

		} while ((lastSeconds != seconds) || (lastMinutes != minutes) || (lastHours != hours) ||
				(lastDay != day) || (lastMonth != month) || (lastYear != year) || (lastCentury != century));

		registerB = readRegister(Registers::B);

		if (!(registerB & 0x04))
		{
			seconds = (seconds & 0x0F) + ((seconds / 16) * 10);
			minutes = (minutes & 0x0F) + ((minutes / 16) * 10);
			hours = ((hours & 0x0F) + (((hours & 0x70) / 16) * 10)) | (hours & 0x80);
			day = (day & 0x0F) + ((day / 16) * 10);
			month = (month & 0x0F) + ((month / 16) * 10);
			year = (year & 0x0F) + ((year / 16) * 10);
			if (centuryRegister)
				century = (century & 0x0F) + ((century / 16) * 10);
		}

		if (!(registerB & 0x02) && (hours & 0x80))
			hours = ((hours & 0x7F) + 12) % 24;

		if (centuryRegister)
			year += century * 100;
		else
		{
			year += (CURRENT_YEAR / 100) * 100;
			if (year < CURRENT_YEAR) year += 100;
		}
	}
}