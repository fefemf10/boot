export module driver.AHCI;
import pci.structures;
import console;
export namespace driver
{
	class AHCI
	{
	public:
		AHCI(pci::Header* baseAddress) : baseAddress(baseAddress)
		{
			console::puts(u8"AHCI Driver instance initialized\n");
		};
		~AHCI()
		{
			delete baseAddress;
		}
	private:
		pci::Header* baseAddress;
	};
}