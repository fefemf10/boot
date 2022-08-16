export module disk.sata;
import types;
import console;
export namespace disk::sata
{
	enum class FIS
	{
		REGH2D = 0x27,
		REGD2H = 0x34,
		DMAACT = 0x39,
		DMASETUP = 0x41,
		DATA = 0x46,
		BIST = 0x58,
		PIOSETUP = 0x5F,
		DEVBITS = 0xA1
	};
	struct REGH2D
	{
		u8 fis;
		u8 pmport : 4;
		u8 rsv0 : 3;
		u8 c : 1;
		u8 command;
		u8 feature0;
		u8 lba0;
		u8 lba1;
		u8 lba2;
		u8 device;
		u8 lba3;
		u8 lba4;
		u8 lba5;
		u8 feature1;
		u16 count;
		u8 icc;
		u8 control;
		u32 rsv1;
	};
	struct REGD2H
	{
		u8 fis;
		u8 pmport : 4;
		u8 rsv0 : 2;
		u8 i : 1;
		u8 rsv1 : 1;
		u8 status;
		u8 error;
		u8 lba0;
		u8 lba1;
		u8 lba2;
		u8 device;
		u8 lba3;
		u8 lba4;
		u8 lba5;
		u8 rsv2;
		u16 count;
		u16 rsv3;
		u32 rsv4;
	};
	struct DATA
	{
		u8 fis;
		u8 pmport : 4;
		u8 rsv0 : 4;
		u16 rsv1;
		u32 data;
	};
	struct PIOSETUP
	{
		u8 fis;
		u8 pmport : 4;
		u8 rsv0 : 1;
		u8 d : 1;
		u8 i : 1;
		u8 rsv1 : 1;
		u8 status;
		u8 error;
		u8 lba0;
		u8 lba1;
		u8 lba2;
		u8 device;
		u8 lba3;
		u8 lba4;
		u8 lba5;
		u8 rsv2;
		u16 count;
		u8 rsv3;
		u8 estatus;
		u16 tc;
		u16 rsv4;
	};
	struct DMASETUP
	{
		u8 fis;
		u8 pmport : 4;
		u8 rsv0 : 1;
		u8 d : 1;
		u8 i : 1;
		u8 a : 1;
		u16 rsv1;
		u64 DMAbufferID;
		u32 rsv2;
		u32 DMAbufferOffset;
		u32 tc;
		u32 rsv3;
	};
	struct HBAPORT
	{
		u32 clb;
		u32 clbu;
		u32 fb;
		u32 fbu;
		u32 is;
		u32 ie;
		u32 cmd;
		u32 rsv0;
		u32 tfd;
		u32 sig;
		u32 ssts;
		u32 sctl;
		u32 serr;
		u32 sact;
		u32 ci;
		u32 sntf;
		u32 fbs;
		u32 rsv1[11];
		u32 vendor[4];
	};
	struct HBAMEM
	{
		u32 cap;
		u32 ghc;
		u32 is;
		u32 pi;
		u32 vs;
		u32 cccCtl;
		u32 cccPts;
		u32 emLoc;
		u32 emCtl;
		u32 cap2;
		u32 bohc;
		u8 rsv[0xA0 - 0x2C];
		u8 vendor[0x100 - 0xA0];
		HBAPORT port;
	};
	struct HBAFIS
	{
		DMASETUP dsfis;
		u32 pad0;
		PIOSETUP psfis;
		u32 pad1[3];
		REGD2H rfis;
		u32 pad2;
		DEVBITS sdbfiis;
		u64 ufis[8];
		u8 rsv[0x100 - 0xA0];
	};
	struct HBACMDHEADER
	{
		u8 clf : 5;
		u8 a : 1;
		u8 w : 1;
		u8 p : 1;
		u8 r : 1;
		u8 b : 1;
		u8 c : 1;
		u8 rsv0 : 1;
		u8 pmport : 4;
		u16 prdtl;
		u32 prdbc;
		u32 ctba;
		u32 ctbau;
		u32 rsv1[4];
	};
	struct HBAPRDTENTRY
	{
		u32 dba;
		u32 dbau;
		u32 rsv0;
		u32 dbc : 22;
		u32 rsv1 : 9;
		u32 i : 1;
	};
	struct HBACMDTBL
	{
		u64 cfis[8];
		u64 acmd[2];
		u64 rsv[6];
		HBAPRDTENTRY prdtEntry;
	};
	enum class ACHISIG
	{
		ATAPI,
		SEMB,
		PM,
		SATA
	};
	enum class ACHIDEV
	{
		NULL,
		SATAPI,
		SEMB,
		PM,
		SATA
	};
	enum class HBAPORTFLAG : u8
	{
		ACTIVE = 0x01,
		PRESENT = 0x03
	};
	ACHIDEV getType(const HBAPORT* port)
	{
		u32 ssts = port->ssts;
		HBAPORTFLAG ipm = static_cast<HBAPORTFLAG>((ssts >> 8) & 0xFF);
		HBAPORTFLAG det = static_cast<HBAPORTFLAG>(ssts & 0x0F);
		if (det != HBAPORTFLAG::PRESENT)
			return ACHIDEV::NULL;
		if (ipm != HBAPORTFLAG::ACTIVE)
			return ACHIDEV::NULL;
		switch (static_cast<ACHISIG>(port->sig))
		{
		case ACHISIG::ATAPI:
			return ACHIDEV::SATAPI;
		case ACHISIG::SEMB:
			return ACHIDEV::SEMB;
		case ACHISIG::PM:
			return ACHIDEV::PM;
		default:
			return ACHIDEV::SATA;
		}
	}
	void probePort(const HBAMEM* abar)
	{
		u32 pi = abar->pi;
		for (u8 i = 0; i < 32; ++i, pi >>= 1)
		{
			if (pi & 1)
			{
				ACHIDEV dt = static_cast<ACHIDEV>(&abar->ports[i]);
				switch (dt)
				{
				case disk::sata::ACHIDEV::SATAPI:
					console::printf(u8"SATAPI drive found at port %d\n", i);
					break;
				case disk::sata::ACHIDEV::SEMB:
					console::printf(u8"SEMB drive found at port %d\n", i);
					break;
				case disk::sata::ACHIDEV::PM:
					console::printf(u8"PM drive found at port %d\n", i);
					break;
				case disk::sata::ACHIDEV::SATA:
					console::printf(u8"SATA drive found at port %d\n", i);
					break;
				default:
					console::printf(u8"No drive found at port %d\n", i);
					break;
				}
			}
		}
	}
}