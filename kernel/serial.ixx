export module serial;
import types;
import intrinsic1;
export namespace serial
{
	constinit const u16 PORT = 0x3f8;
	void initialize()
	{
		__outbyte(PORT + 1, 0x00);
		__outbyte(PORT + 3, 0x80);
		__outbyte(PORT + 0, 0x01);
		__outbyte(PORT + 1, 0x00);
		__outbyte(PORT + 3, 0x03);
		__outbyte(PORT + 2, 0xC7);
		__outbyte(PORT + 4, 0x0B);
		__outbyte(PORT + 4, 0x1E);
		__outbyte(PORT + 0, 0xAE);
		__outbyte(PORT + 4, 0x0F);
	}
	u8 read()
	{
		while ((__inbyte(PORT + 5) & 1) == 0);
		return __inbyte(PORT);
	}
	void write(u8 data)
	{
		while ((__inbyte(PORT + 5) & 0x20) == 0);
		__outbyte(PORT, data);
	}
	void puthex(const void* data, u64 size)
	{
		for (size_t i = 0; i < size; i++)
		{
			serial::write(reinterpret_cast<const u8*>(data)[i]);
		}
	}
}