export module fs.FAT.print;
import console;
import fs.FAT.structures;
import sl.bit;
export namespace fs::FAT
{
	void print(const FATDate& date)
	{
		console::printf("%02i.%02i.%04i", date.day, date.month, date.year + 1980);
	}
	void print(const FATTime& time)
	{
		console::printf("%02i:%02i:%02i", time.hours, time.minutes, time.seconds * 2);
	}
}