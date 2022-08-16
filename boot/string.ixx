export module string;
import types;
export namespace string
{
	char8_t buffer[9]{};
	const char8_t* itos(u64 number, i32 radix)
	{
		i8 buf[32]{};
		i8 pos = 0;
		const char8_t hexchar8_ts[] = u8"0123456789ABCDEF";
		// convert number to ASCII
		do
		{
			u64 rem = number % radix;
			number /= radix;
			buf[pos++] = hexchar8_ts[rem];
		} while (number > 0);

		// print number in reverse order
		u8 i{};
		while (--pos >= 0)
			buffer[i++] = static_cast<char8_t>(buf[pos]);
		return buffer;
	}
}