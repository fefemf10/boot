export module driver.AHCI;
export import driver.AHCI.structures;
import pci.structures;
import types;
import console;
import memory;
import sl.utility;
import string;
import cpuio;
import PIT;
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
		HBAPORT* port;
		AHCIDEV portType;
		u8* buffer;
		u8 portNumber;
		u8 HBAportCount;
		void configure()
		{
			stopCMD();
			HBACMDHEADER* cmdHeaders = reinterpret_cast<HBACMDHEADER*>(memory::allocator::allocBlocks(1));
			memory::pageTableManager.mapMemory(cmdHeaders, cmdHeaders);
			port->clb = reinterpret_cast<u64>(cmdHeaders);
			memory::set(cmdHeaders, 0, 32 * sizeof(HBACMDHEADER));

			void* fisBase = memory::allocator::allocBlocks(1);
			memory::pageTableManager.mapMemory(fisBase, fisBase);
			port->fb = reinterpret_cast<u64>(fisBase);
			memory::set(fisBase, 0, sizeof(HBAFIS));

			void* cmdTableAddress = memory::allocator::allocBlocks(2);
			memory::pageTableManager.mapMemory(cmdTableAddress, cmdTableAddress);
			memory::pageTableManager.mapMemory(reinterpret_cast<u8*>(cmdTableAddress) + 4096, reinterpret_cast<u8*>(cmdTableAddress) + 4096);
			memory::set(cmdTableAddress, 0, 32 * sizeof(HBACMDTBL));
			for (size_t i = 0; i < 32; i++)
			{
				cmdHeaders[i].prdtl = maxCountPRDTEntry;
				cmdHeaders[i].ctba = reinterpret_cast<u64>(cmdTableAddress) + i * sizeof(HBACMDTBL);
			}
			
			startCMD();
			port->is = 0;
			port->ie = 0xFFFFFFFF;
		}
		void startCMD()
		{
			while (port->cmd.cr)
			{
				console::printf(u8"a");
				cpuio::pause();
			}
			port->cmd.fre = 1;
			port->cmd.s = 1;
		}
		void stopCMD()
		{
			port->cmd.s = 0;
			while (port->cmd.cr)
			{
				cpuio::pause();
			}
			if (port->cmd.fre)
			{
				port->cmd.fre = 0;
				while (port->cmd.fr)
				{
					cpuio::pause();
				}
			}
		}
		bool read(const u64 sector, const u16 sectorCount, void* buffer)
		{
			port->is = 0xFFFFFFFF;
			const i32 slot = findSlot();
			if (slot == -1)
				return false;
			HBACMDHEADER* cmdHeaders = reinterpret_cast<HBACMDHEADER*>(port->clb);
			cmdHeaders[slot].clf = sizeof(REGH2D) / sizeof(u32);
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
					cmdTable->prdtEntry[i].i = 0;
					count -= 16;
				}
				cmdTable->prdtEntry[i].dba = reinterpret_cast<u64>(buffer) + i * 0x2000;
				cmdTable->prdtEntry[i].dbc = (count << 9) - 1;
				cmdTable->prdtEntry[i].i = 0;
			}

			REGH2D* cmdFIS = reinterpret_cast<REGH2D*>(&cmdTable->cfis);
			cmdFIS->fis = std::to_underlying(FIS::REGH2D);
			cmdFIS->control = 1;
			cmdFIS->c = 1;
			cmdFIS->command = std::to_underlying(ATA::CMDREADDMAEX);
			cmdFIS->lba0 = static_cast<u8>(sector & 0xFF);
			cmdFIS->lba1 = static_cast<u8>(sector >> 8 & 0xFF);
			cmdFIS->lba2 = static_cast<u8>(sector >> 16 & 0xFF);
			cmdFIS->device = 1 << 6; //LBA Mode
			cmdFIS->lba3 = static_cast<u8>(sector >> 24 & 0xFF);
			cmdFIS->lba4 = static_cast<u8>(sector >> 32 & 0xFF);
			cmdFIS->lba5 = static_cast<u8>(sector >> 40 & 0xFF);
			cmdFIS->count = (u16)sectorCount;
			
			while (port->tfd & std::to_underlying(ATA::BUSY) || port->tfd & std::to_underlying(ATA::DRQ))
			{
				cpuio::pause();
			}
			port->ci = 1 << slot;
			while (port->ci & (1 << slot))
			{
				cpuio::pause();
			}
			/*while (true)
			{
				if ((port->ci & (1 << slot)) == 0) break;
				if (port->is & std::to_underlying(HBAPxISe::TFES))
				{
					console::printf(u8"Read disk error\n");
					return false;
				}
			}*/
			return true;
		}
		i32 findSlot()
		{
			u32 slots = port->sact | port->ci;
			for (u32 i = 0; i < HBAportCount; i++)
			{
				if ((slots & 1) == 0)
					return i;
				slots >>= 1;
			}
			return -1;
		}
		bool identify(void* buf)
		{
			port->is = 0xFFFFFFFF;
			const i32 slot = findSlot();
			if (slot == -1)
				return false;
			HBACMDHEADER* cmdHeaders = reinterpret_cast<HBACMDHEADER*>(port->clb);
			cmdHeaders[slot].clf = sizeof(REGH2D) / sizeof(u32);
			cmdHeaders[slot].w = 0;

			HBACMDTBL* cmdTable = reinterpret_cast<HBACMDTBL*>(cmdHeaders[slot].ctba);
			cmdTable->prdtEntry[0].dba = reinterpret_cast<u64>(buf);
			cmdTable->prdtEntry[0].dbc = 511;
			cmdTable->prdtEntry[0].i;

			REGH2D* cmdFIS = reinterpret_cast<REGH2D*>(&cmdTable->cfis);
			cmdFIS->fis = std::to_underlying(FIS::REGH2D);
			cmdFIS->c = 1;
			cmdFIS->device = 1 << 6;
			cmdFIS->command = std::to_underlying(ATA::CMDIDENTIFYDEVICE);
			u64 spin{};
			while ((port->tfd & (std::to_underlying(ATA::BUSY) | std::to_underlying(ATA::DRQ))) && spin < 1000000)
			{
				++spin;
			}
			if (spin == 1000000)
			{
				return false;
			}
			port->ci = 1 << slot;
			while (true)
			{
				if ((port->ci & 1 << slot) == 0) break;
				if (port->is & std::to_underlying(HBAPxISe::TFES))
				{
					return false;
				}
			}
			if (port->is & std::to_underlying(HBAPxISe::TFES))
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
			memory::pageTableManager.mapMemory(baseAddress, baseAddress);
			abar = reinterpret_cast<HBAMEM*>(static_cast<u64>(reinterpret_cast<pci::Header0*>(baseAddress)->bars[5]));
			memory::pageTableManager.mapMemory(abar, abar);
			baseAddress->cmd.bme = 1;
			baseAddress->cmd.mse = 1;
			baseAddress->cmd.id = 0;
			abar->ghc.ae = 1;
			/*abar->ghc.hr = 1;
			while (abar->ghc.hr)
			{
				cpuio::iowait();
			}
			abar->ghc.ae = 1;*/
			probePort();
			for (size_t i = 0; i < portCount; i++)
			{
				Port* port = ports[i];
				//console::printf(u8"%x %x %x %x\n", port->port->cmd.s = 0, port->port->cmd.cr = 0, port->port->cmd.fr = 0, port->port->cmd.fre = 0);
				port->configure();
				
				ATAIDENTIFYDATA buf;

				port->identify(&buf);
				console::setOut(console::OUT::SERIAL);
				console::puth(&buf, 512);
				console::puts(u8"\n");
				console::setOut(console::OUT::TELETYPE);
				char8_t name[41]{};
				{
					for (size_t k = 0; k < 40; k += 2)
					{
						name[k] = buf.ModelNumber[k + 1];
						name[k + 1] = buf.ModelNumber[k];
					}
					i64 m = 39;

					for (; m > 0; m--)
					{
						if (name[m] == 0x20)
						{
							name[m] = u8'\0';
						}
						else
							break;
					}
				}

				console::printf(u8"[AHCI] Detected SATA: %s (%llu MiB)\n", name, buf.Max48BitLBA / 2048);
				port->buffer = (u8*)memory::allocator::allocBlocks(1);
				memory::pageTableManager.mapMemory(port->buffer, port->buffer);
				memory::set(port->buffer, 0, 0x1000);
				port->buffer[0] = 5;
				console::setOut(console::OUT::SERIAL);
				if (port->read(0, 8, port->buffer))
					console::puts(u8"Success\n");
				
				/*console::setOut(console::OUT::SERIAL);
				console::puth(reinterpret_cast<u64*>(port->port->clb), 32 * sizeof(HBACMDHEADER));
				console::puts(u8"\n");
				console::puth(reinterpret_cast<HBACMDTBL*>(reinterpret_cast<HBACMDHEADER*>(port->port->clb)->ctba), 256 * 32);
				console::puts(u8"\n");
				console::setOut(console::OUT::TELETYPE);
				HBAFIS* dfis = reinterpret_cast<HBAFIS*>(port->port->fb);
				console::printf(u8"%hx ", (u16)(dfis->rfis.error));*/
				//console::printf(u8"%x \n", abar->ghc);
				console::puth(port->buffer, 0x1000);
				console::setOut(console::OUT::TELETYPE);
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
					if (dt == AHCIDEV::SATA)
					{
						ports[portCount] = new Port();
						ports[portCount]->portType = dt;
						ports[portCount]->port = &abar->ports[i];
						ports[portCount]->portNumber = portCount;
						ports[portCount]->HBAportCount = abar->cap.np;
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