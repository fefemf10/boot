export module driver.AHCI.structures;
import types;
export namespace driver
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
		u64 clb;
		u64 fb;
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
	struct HBACAP
	{
		u32 np : 5;
		u32 sxs : 1;
		u32 ems : 1;
		u32 cccs : 1;
		u32 ncs : 5;
		u32 psc : 1;
		u32 ssc : 1;
		u32 pmd : 1;
		u32 fbss : 1;
		u32 spm : 1;
		u32 sam : 1;
		u32 rsv : 1;
		u32 iss : 4;
		u32 sclo : 1;
		u32 sal : 1;
		u32 salp : 1;
		u32 sss : 1;
		u32 smps : 1;
		u32 ssntf : 1;
		u32 sncq : 1;
		u32 s64a : 1;
	};
	struct HBAGHC
	{
		u32 hr : 1;
		u32 ie : 1;
		u32 mrsm : 1;
		u32 rsv : 28;
		u32 ae : 1;
	};
	struct HBAEMLOC
	{
		u16 sz;
		u16 ofst;
	};
	struct HBAEMCTL
	{
		u32 stsmr : 1;
		u32 rsv0 : 7;
		u32 ctltm : 1;
		u32 ctlrst : 1;
		u32 rsv1 : 6;
		u32 suppled : 1;
		u32 suppsafte : 1;
		u32 suppses2 : 1;
		u32 suppsgpio : 1;
		u32 rsv2 : 4;
		u32 attrsmb : 1;
		u32 attrxtm : 1;
		u32 attralhd : 1;
		u32 attrpm : 1;
		u32 rsv3 : 4;
	};
	struct HBACAP2
	{
		u32 boh : 1;
		u32 nvmp : 1;
		u32 apst : 1;
		u32 sds : 1;
		u32 sadm : 1;
		u32 deso : 1;
		u32 rsv : 26;
	};
	struct HBABOHC
	{
		u32 bos : 1;
		u32 oos : 1;
		u32 sooe : 1;
		u32 ooc : 1;
		u32 bb : 1;
		u32 rsv : 27;
	};
	struct HBAMEM
	{
		HBACAP cap;
		HBAGHC ghc;
		u32 is;
		u32 pi;
		u32 vs;
		u32 cccCtl;
		u32 cccPts;
		HBAEMLOC emLoc;
		HBAEMCTL emCtl;
		HBACAP2 cap2;
		HBABOHC bohc;
		u8 rsv[0xA0 - 0x2C];
		u8 vendor[0x100 - 0xA0];
		HBAPORT ports[1];
	};
	struct HBAFIS
	{
		DMASETUP dsfis;
		u32 pad0;
		PIOSETUP psfis;
		u32 pad1[3];
		REGD2H rfis;
		u32 pad2;
		//DEVBITS sdbfiis;
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
		u64 rsv1 : 7;
		u64 ctba : 57;
		u32 rsv2[4];
	};
	struct HBAPRDTENTRY
	{
		u64 dba;
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
		HBAPRDTENTRY* prdtEntry;
	};
	enum class ACHISIG : u32
	{
		ATAPI = 0xEB140101,
		SEMB = 0xC33C0101,
		PM = 0x96690101,
		SATA = 0x101
	};
	enum class AHCIDEV
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
	enum class HBAPxCMD
	{
		CR = 0x8000,
		FRE = 0x0010,
		ST = 0x0001,
		FR = 0x4000,
	};
	enum class ATA
	{
		BUSY = 0x80,
		DRQ = 0x08,
		CMDREADDMAEX = 0x25
	};
	enum class HBAPxIS
	{
		TFES = (1 << 30)
	};
}