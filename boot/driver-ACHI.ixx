export module driver.ACHI;
import pci.structures;
import console;
export namespace driver
{
	class ACHI
	{
	public:
		ACHI(pci::Header* baseAddress) : baseAddress(baseAddress)
		{
			console::puts(u8"ACHI Driver instance initialized\n");
		};
		~ACHI()
		{
			delete baseAddress;
		}
	private:
		pci::Header* baseAddress;
	};
}