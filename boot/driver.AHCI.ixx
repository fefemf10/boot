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
			HBACMDHEADER* cmdHeader = reinterpret_cast<HBACMDHEADER*>(memory::allocator::allocBlocks(1));
			memory::pageTableManager.mapMemory(cmdHeader, cmdHeader);
			HBAPort->clb = reinterpret_cast<u64>(cmdHeader);
			memory::set(cmdHeader, 0, 32 * sizeof(HBACMDHEADER));
			for (size_t i = 0; i < 32; i++)
			{
				cmdHeader[i].prdtl = maxCountPRDTEntry;
				void* cmdTableAddress = memory::allocator::allocBlocks(1);
				memory::pageTableManager.mapMemory(cmdTableAddress, cmdTableAddress);
				cmdHeader[i].ctba = reinterpret_cast<u64>(cmdTableAddress);
				memory::set(cmdTableAddress, 0, sizeof(HBACMDTBL));
			}
			void* fisBase = memory::allocator::allocBlocks(1);
			memory::pageTableManager.mapMemory(fisBase, fisBase);
			HBAPort->fb = reinterpret_cast<u64>(fisBase);
			memory::set(fisBase, 0, 256);
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
		bool read(const u64 sector, const u16 sectorCount, void* buffer)
		{
			HBAPort->is = -1u;
			const i32 slot = findSlot();
			if (slot == -1)
				return false;
			HBACMDHEADER* cmdHeaders = reinterpret_cast<HBACMDHEADER*>(HBAPort->clb);
			cmdHeaders[slot].clf = sizeof(REGH2D)/sizeof(u32);
			cmdHeaders[slot].w = 0;//read
			cmdHeaders[slot].prdtl = ((sectorCount - 1) >> 4) + 1;
			
			HBACMDTBL* cmdTable = reinterpret_cast<HBACMDTBL*>(cmdHeaders[slot].ctba);
			memory::set(cmdTable, 0, sizeof(HBACMDTBL));
			{
				u16 count = sectorCount;
				i64 i = 0;
				for (; i < cmdHeaders[slot].prdtl - 1; i++)
				{
					cmdTable->prdtEntry[i].dba = reinterpret_cast<u64>(buffer) + i * 0x2000;
					cmdTable->prdtEntry[i].dbc = 0x2000 - 1;
					cmdTable->prdtEntry[i].i = 1;
					count -= 16;
				}
				cmdTable->prdtEntry[i].dba = reinterpret_cast<u64>(buffer) + i * 0x2000;
				cmdTable->prdtEntry[i].dbc = (count << 9) - 1;
				cmdTable->prdtEntry[i].i = 1;
			}
			
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
			HBAPort->ci = 1 << slot;
			while (true)
			{
				if ((HBAPort->ci & 1 << slot) == 0) break;
				if (HBAPort->is & std::to_underlying(HBAPxIS::TFES))
				{
					return false;
				}
			}
			if (HBAPort->is & std::to_underlying(HBAPxIS::TFES))
			{
				return false;
			}
			return true;
		}
		i32 findSlot()
		{
			u32 slots = HBAPort->sact | HBAPort->ci;
			for (u32 i = 0; i < 32; i++)
			{
				if ((slots & 1) == 0)
					return i;
				slots >>= 1;
			}
			return -1;
		}
		bool identify(void* buf)
		{
			HBAPort->is = -1u;
			const i32 slot = findSlot();
			if (slot == -1)
				return false;
			HBACMDHEADER* cmdHeaders = reinterpret_cast<HBACMDHEADER*>(HBAPort->clb);
			cmdHeaders[slot].clf = sizeof(REGH2D) / sizeof(u32);
			cmdHeaders[slot].w = 0;

			HBACMDTBL* cmdTable = reinterpret_cast<HBACMDTBL*>(cmdHeaders[slot].ctba);
			cmdTable->prdtEntry[0].dba = reinterpret_cast<u64>(buf);
			cmdTable->prdtEntry[0].dbc = 512;
			cmdTable->prdtEntry[0].i;
			
			REGH2D* cmdFIS = reinterpret_cast<REGH2D*>(&cmdTable->cfis);
			cmdFIS->fis = std::to_underlying(FIS::REGH2D);
			cmdFIS->c = 1;
			cmdFIS->device = 1 << 6;
			cmdFIS->command = std::to_underlying(ATA::CMDIDENTIFYDEVICE);
			u64 spin{};
			while ((HBAPort->tfd & (std::to_underlying(ATA::BUSY) | std::to_underlying(ATA::DRQ))) && spin < 1000000)
			{
				++spin;
			}
			if (spin == 1000000)
			{
				return false;
			}
			HBAPort->ci = 1 << slot;
			while (true)
			{
				if ((HBAPort->ci & 1 << slot) == 0) break;
				if (HBAPort->is & std::to_underlying(HBAPxIS::TFES))
				{
					return false;
				}
			}
			if (HBAPort->is & std::to_underlying(HBAPxIS::TFES))
			{
				return false;
			}
			return true;
		}
	};
	class AHCI
	{
	public:
		AHCI(pci::Header* baseAddress) : baseAddress(baseAddress)
		{
			console::puts(u8"AHCI Driver instance initialized\n");
			abar = reinterpret_cast<HBAMEM*>(static_cast<u64>(reinterpret_cast<pci::Header0*>(baseAddress)->bars[5]));
			memory::pageTableManager.mapMemory(abar, abar);
			probePort();
			//console::printf(u8"%llx ", abar);
			//abar->ghc.ie = 1;
			for (size_t i = 0; i < portCount; i++)
			{
				Port* port = ports[i];
				port->configure();
				ATAIDENTIFYDATA* buf = (ATAIDENTIFYDATA*)memory::allocator::allocBlocks(1);
				memory::pageTableManager.mapMemory(buf, buf);
				memory::set(buf, 0, 0x200);
				
				port->identify(buf);
				console::setOut(console::OUT::SERIAL);
				console::puth(buf, 512);
				console::setOut(console::OUT::TELETYPE);
				port->buffer = (u8*)memory::allocator::allocBlocks(1);
				memory::pageTableManager.mapMemory(port->buffer, port->buffer);
				memory::set(port->buffer, 0, 0x1000);
				if (port->read(0, 1, port->buffer))
					console::puts(u8"Success\n");
				
				//console::printf(u8"%x \n", abar->ghc);
				console::puth(port->buffer, 512);
				/*for (size_t j = 0; j < 1024; j++)
				{
					console::puts(string::itos(port->buffer[j], 10));
				}*/
				console::puts(u8"\n");
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