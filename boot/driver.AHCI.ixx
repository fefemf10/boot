export module driver.AHCI;
export import driver.AHCI.structures;
import pci.structures;
import types;
import console;
import memory;
import sl.utility;
import string;
export namespace driver
{
	AHCIDEV getType(const HBAPORT& port)
	{
		const u32 ssts = port.ssts;
		HBAPORTFLAG ipm = static_cast<HBAPORTFLAG>((ssts >> 8) & 0xFF);
		HBAPORTFLAG det = static_cast<HBAPORTFLAG>(ssts & 0x0F);
		if (det != HBAPORTFLAG::PRESENT)
			return AHCIDEV::NULL;
		if (ipm != HBAPORTFLAG::ACTIVE)
			return AHCIDEV::NULL;
		switch (static_cast<ACHISIG>(port.sig))
		{
		case ACHISIG::ATAPI:
			return AHCIDEV::SATAPI;
		case ACHISIG::SEMB:
			return AHCIDEV::SEMB;
		case ACHISIG::PM:
			return AHCIDEV::PM;
		case ACHISIG::SATA:
			return AHCIDEV::SATA;
		default:
			return AHCIDEV::NULL;
		}
	}
	
	struct Port
	{
		HBAPORT* HBAPort;
		AHCIDEV portType;
		u8* buffer;
		u8 portNumber;
		void configure()
		{
			stopCMD();
			void* newBase = memory::allocator::allocBlocks(1);
			memory::pageTableManager.mapMemory(newBase, newBase);
			HBAPort->clb = reinterpret_cast<u64>(newBase);
			memory::set(newBase, 0, 1024);

			void* fisBase = memory::allocator::allocBlocks(1);
			memory::pageTableManager.mapMemory(fisBase, fisBase);
			HBAPort->fb = reinterpret_cast<u64>(fisBase);
			memory::set(fisBase, 0, 256);

			HBACMDHEADER* cmdHeader = reinterpret_cast<HBACMDHEADER*>(HBAPort->clb);
			for (size_t i = 0; i < 32; i++)
			{
				cmdHeader[i].prdtl = 8;
				void* cmdTableAddress = memory::allocator::allocBlocks(1);
				memory::pageTableManager.mapMemory(cmdTableAddress, cmdTableAddress);
				u64 address = reinterpret_cast<u64>(cmdTableAddress) + (i << 8);
				cmdHeader[i].ctba = address;
				memory::set(cmdTableAddress, 0, 256);
			}
			startCMD();
		}
		void startCMD()
		{
			while (HBAPort->cmd & std::to_underlying(HBAPxCMD::CR));
			HBAPort->cmd |= std::to_underlying(HBAPxCMD::FRE);
			HBAPort->cmd |= std::to_underlying(HBAPxCMD::ST);
		}
		void stopCMD()
		{
			HBAPort->cmd &= ~std::to_underlying(HBAPxCMD::ST);
			HBAPort->cmd &= ~std::to_underlying(HBAPxCMD::FRE);
			while (true)
			{
				if (HBAPort->cmd & std::to_underlying(HBAPxCMD::FR)) continue;
				if (HBAPort->cmd & std::to_underlying(HBAPxCMD::CR)) continue;
				break;
			}
		}
		bool read(u64 sector, u16 sectorCount, void* buffer)
		{
			HBAPort->is = u32(-1);
			HBACMDHEADER* cmdHeader = reinterpret_cast<HBACMDHEADER*>(HBAPort->clb);
			cmdHeader->clf = sizeof(REGH2D)/sizeof(u32);
			cmdHeader->w = 0;//read
			cmdHeader->prdtl = 1;

			HBACMDTBL* cmdTable = reinterpret_cast<HBACMDTBL*>(cmdHeader->ctba);
			memory::set(cmdTable, 0, sizeof(HBACMDTBL) + (cmdHeader->prdtl-1)*sizeof(HBAPRDTENTRY));

			cmdTable->prdtEntry[0].dba = reinterpret_cast<u64>(buffer);
			cmdTable->prdtEntry[0].dbc = (sectorCount << 9) - 1;
			cmdTable->prdtEntry[0].i = 1;
			REGH2D* cmdFIS = reinterpret_cast<REGH2D*>(&cmdTable->cfis);
			cmdFIS->fis = std::to_underlying(FIS::REGH2D);
			cmdFIS->control = 1;
			cmdFIS->command = std::to_underlying(ATA::CMDREADDMAEX);
			cmdFIS->lba0 = static_cast<u8>(sector);
			cmdFIS->lba1 = static_cast<u8>(sector >> 8);
			cmdFIS->lba2 = static_cast<u8>(sector >> 16);
			cmdFIS->lba3 = static_cast<u8>(sector >> 24);
			cmdFIS->lba4 = static_cast<u8>(sector >> 32);
			cmdFIS->lba5 = static_cast<u8>(sector >> 40);
			cmdFIS->device = 1 << 6;//LBA Mode
			cmdFIS->count = (u16)sectorCount;
			u64 spin{};
			while ((HBAPort->tfd & (std::to_underlying(ATA::BUSY) | std::to_underlying(ATA::DRQ))) && spin < 1000000)
			{
				++spin;
			}
			if (spin == 1000000)
			{
				return false;
			}
			HBAPort->ci = 1;
			while (true)
			{
				if (HBAPort->ci == 0) break;
				if (HBAPort->is & std::to_underlying(HBAPxIS::TFES))
				{
					return false;
				}
			}
			return true;
		}
		i32 findSlot()
		{
			u32 slots = HBAPort->sact | HBAPort->ci;
			for (size_t i = 0; i < 32; i++)
			{
				if ((slots & 1) == 0)
					return 0;
				slots >>= 1;
			}
			return -1;
		}
	};
	class AHCI
	{
	public:
		AHCI(pci::Header* baseAddress) : baseAddress(baseAddress)
		{
			console::puts(u8"AHCI Driver instance initialized\n");
			abar = reinterpret_cast<HBAMEM*>(reinterpret_cast<pci::Header0*>(baseAddress)->bars[5]);
			memory::pageTableManager.mapMemory(abar, abar);
			probePort();
			console::printf(u8"%llx ", abar);
			for (size_t i = 0; i < portCount; i++)
			{
				Port* port = ports[i];
				port->configure();
				port->buffer = (u8*)memory::allocator::allocBlocks(1);
				memory::pageTableManager.mapMemory(port->buffer, port->buffer);
				memory::set(port->buffer, 0, 0x1000);
				port->read(0, 4, port->buffer);
				
				for (size_t j = 0; j < 1024; j++)
				{
					console::puts(string::itos(port->buffer[i], 10));
				}
			}
		};
		~AHCI()
		{

		}
		void probePort()
		{
			u32 pi = abar->pi;
			for (u8 i = 0; i < abar->cap.np; ++i, pi >>= 1)
			{
				if (pi & 1)
				{
					AHCIDEV dt = getType(abar->ports[i]);
					if (dt == AHCIDEV::SATA || dt == AHCIDEV::SATAPI)
					{
						ports[portCount] = new Port();
						ports[portCount]->portType = dt;
						ports[portCount]->HBAPort = &abar->ports[i];
						ports[portCount]->portNumber = portCount;
						++portCount;
					}
				}
			}
		}
		void printStats()
		{
			console::printf(u8"S64A %x\n", abar->cap.s64a);
			console::printf(u8"SNCQ %x\n", abar->cap.sncq);
			console::printf(u8"SSNTF %x\n", abar->cap.ssntf);
			console::printf(u8"SMPS %x\n", abar->cap.smps);
			console::printf(u8"SSS %x\n", abar->cap.sss);
			console::printf(u8"SALP %x\n", abar->cap.salp);
			console::printf(u8"SAL %x\n", abar->cap.sal);
			console::printf(u8"SCLO %x\n", abar->cap.sclo);
			console::printf(u8"ISS %x\n", abar->cap.iss);
			console::printf(u8"SAM %x\n", abar->cap.sam);
			console::printf(u8"SPM %x\n", abar->cap.spm);
			console::printf(u8"FBSS %x\n", abar->cap.fbss);
			console::printf(u8"PMD %x\n", abar->cap.pmd);
			console::printf(u8"SSC %x\n", abar->cap.ssc);
			console::printf(u8"PSC %x\n", abar->cap.psc);
			console::printf(u8"NCS %x\n", abar->cap.ncs);
			console::printf(u8"CCCS %x\n", abar->cap.cccs);
			console::printf(u8"EMS %x\n", abar->cap.ems);
			console::printf(u8"SXS %x\n", abar->cap.sxs);
			console::printf(u8"NP %x\n", abar->cap.np);
		}
	private:
		pci::Header* baseAddress;
		HBAMEM* abar;
		Port* ports[32];
		u8 portCount{};
	};
}