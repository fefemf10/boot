export module FADT;
import types;
import SDT;
import GAS;
export namespace ACPI
{
	struct FADT
	{
		SDT header;
		u32 firmwareCtrl;
		u32 DSDT;
		u8 rsv;
		u8 preferredPMProfile;
		u16 SCIInterrupt;
		u32 SMICommand;
		u8 ACPIEnable;
		u8 ACPIDisable;
		u8 S4BIOSReq;
		u8 PSTATECount;
		u32 PM1aEventBlock;
		u32 PM1bEventBlock;
		u32 PM1aCountBlock;
		u32 PM1bCountBlock;
		u32 PM2CountBlock;
		u32 PMTimerBlock;
		u32 GPE0Block;
		u32 GPE1Block;
		u8 PM1EventLength;
		u8 PM1CountLength;
		u8 PM2CountLength;
		u8 PMTimerLength;
		u8 GPE0BlockLength;
		u8 GPE1BlockLength;
		u8 GPE1Base;
		u8 CSTCount;
		u16 P_LVL2Latency;
		u16 P_LVL3Latency;
		u16 flushSize;
		u16 flushStride;
		u8 dutyOffset;
		u8 dutyWidth;
		u8 dayAlarm;
		u8 monthAlarm;
		u8 centry;
		u16 IAPCBootArch;
		u8 rsv1;
		u32 flags;
		GAS resetRegister;
		u8 resetValue;
		u16 ArmBootArch;
		u8 FADTMinorVersion;
		u64 XFirmwareControl;
		u64 XDSDT;
		GAS XPM1aEventBlock;
		GAS XPM1bEventBlock;
		GAS XPM1aControlBlock;
		GAS XPM2ControlBlock;
		GAS XPMTimerBlock;
		GAS XGPE0Block;
		GAS XGPE1Block;
		GAS sleepControlRegister;
		GAS sleepStatusRegister;
		u64 hypervisorVendorIdentity;
	} *fadt;
}