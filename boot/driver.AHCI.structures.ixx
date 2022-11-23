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
		u64 ctba;
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
	constexpr u64 maxCountPRDTEntry = 8;
	struct HBACMDTBL
	{
		u64 cfis[8];
		u64 acmd[2];
		u64 rsv[6];
		HBAPRDTENTRY prdtEntry[maxCountPRDTEntry];
	};
	struct ATAIDENTIFYDATA {
		struct {
			u16 Reserved1 : 1;
			u16 Retired3 : 1;
			u16 ResponseIncomplete : 1;
			u16 Retired2 : 3;
			u16 FixedDevice : 1;
			u16 RemovableMedia : 1;
			u16 Retired1 : 7;
			u16 DeviceType : 1;
		} GeneralConfiguration;
		u16 NumCylinders;
		u16 SpecificConfiguration;
		u16 NumHeads;
		u16 Retired1[2];
		u16 NumSectorsPerTrack;
		u16 VendorUnique1[3];
		u8 SerialNumber[20];
		u16 Retired2[2];
		u16 Obsolete1;
		u8 FirmwareRevision[8];
		u8 ModelNumber[40];
		u8 MaximumBlockTransfer;
		u8 VendorUnique2;
		struct {
			u16 FeatureSupported : 1;
			u16 Reserved : 15;
		} TrustedComputing;
		struct {
			u8 CurrentLongPhysicalSectorAlignment : 2;
			u8 ReservedByte49 : 6;
			u8 DmaSupported : 1;
			u8 LbaSupported : 1;
			u8 IordyDisable : 1;
			u8 IordySupported : 1;
			u8 Reserved1 : 1;
			u8 StandybyTimerSupport : 1;
			u8 Reserved2 : 2;
			u16 ReservedWord50;
		} Capabilities;
		u16 ObsoleteWords51[2];
		u16 TranslationFieldsValid : 3;
		u16 Reserved3 : 5;
		u16 FreeFallControlSensitivity : 8;
		u16 NumberOfCurrentCylinders;
		u16 NumberOfCurrentHeads;
		u16 CurrentSectorsPerTrack;
		u32 CurrentSectorCapacity;
		u8 CurrentMultiSectorSetting;
		u8 MultiSectorSettingValid : 1;
		u8 ReservedByte59 : 3;
		u8 SanitizeFeatureSupported : 1;
		u8 CryptoScrambleExtCommandSupported : 1;
		u8 OverwriteExtCommandSupported : 1;
		u8 BlockEraseExtCommandSupported : 1;
		u32 UserAddressableSectors;
		u16 ObsoleteWord62;
		u16 MultiWordDMASupport : 8;
		u16 MultiWordDMAActive : 8;
		u16 AdvancedPIOModes : 8;
		u16 ReservedByte64 : 8;
		u16 MinimumMWXferCycleTime;
		u16 RecommendedMWXferCycleTime;
		u16 MinimumPIOCycleTime;
		u16 MinimumPIOCycleTimeIORDY;
		struct {
			u16 ZonedCapabilities : 2;
			u16 NonVolatileWriteCache : 1;
			u16 ExtendedUserAddressableSectorsSupported : 1;
			u16 DeviceEncryptsAllUserData : 1;
			u16 ReadZeroAfterTrimSupported : 1;
			u16 Optional28BitCommandsSupported : 1;
			u16 IEEE1667 : 1;
			u16 DownloadMicrocodeDmaSupported : 1;
			u16 SetMaxSetPasswordUnlockDmaSupported : 1;
			u16 WriteBufferDmaSupported : 1;
			u16 ReadBufferDmaSupported : 1;
			u16 DeviceConfigIdentifySetDmaSupported : 1;
			u16 LPSAERCSupported : 1;
			u16 DeterministicReadAfterTrimSupported : 1;
			u16 CFastSpecSupported : 1;
		} AdditionalSupported;
		u16 ReservedWords70[5];
		u16 QueueDepth : 5;
		u16 ReservedWord75 : 11;
		struct {
			u16 Reserved0 : 1;
			u16 SataGen1 : 1;
			u16 SataGen2 : 1;
			u16 SataGen3 : 1;
			u16 Reserved1 : 4;
			u16 NCQ : 1;
			u16 HIPM : 1;
			u16 PhyEvents : 1;
			u16 NcqUnload : 1;
			u16 NcqPriority : 1;
			u16 HostAutoPS : 1;
			u16 DeviceAutoPS : 1;
			u16 ReadLogDMA : 1;
			u16 Reserved2 : 1;
			u16 CurrentSpeed : 3;
			u16 NcqStreaming : 1;
			u16 NcqQueueMgmt : 1;
			u16 NcqReceiveSend : 1;
			u16 DEVSLPtoReducedPwrState : 1;
			u16 Reserved3 : 8;
		} SerialAtaCapabilities;
		struct {
			u16 Reserved0 : 1;
			u16 NonZeroOffsets : 1;
			u16 DmaSetupAutoActivate : 1;
			u16 DIPM : 1;
			u16 InOrderData : 1;
			u16 HardwareFeatureControl : 1;
			u16 SoftwareSettingsPreservation : 1;
			u16 NCQAutosense : 1;
			u16 DEVSLP : 1;
			u16 HybridInformation : 1;
			u16 Reserved1 : 6;
		} SerialAtaFeaturesSupported;
		struct {
			u16 Reserved0 : 1;
			u16 NonZeroOffsets : 1;
			u16 DmaSetupAutoActivate : 1;
			u16 DIPM : 1;
			u16 InOrderData : 1;
			u16 HardwareFeatureControl : 1;
			u16 SoftwareSettingsPreservation : 1;
			u16 DeviceAutoPS : 1;
			u16 DEVSLP : 1;
			u16 HybridInformation : 1;
			u16 Reserved1 : 6;
		} SerialAtaFeaturesEnabled;
		u16 MajorRevision;
		u16 MinorRevision;
		struct {
			u16 SmartCommands : 1;
			u16 SecurityMode : 1;
			u16 RemovableMediaFeature : 1;
			u16 PowerManagement : 1;
			u16 Reserved1 : 1;
			u16 WriteCache : 1;
			u16 LookAhead : 1;
			u16 ReleaseInterrupt : 1;
			u16 ServiceInterrupt : 1;
			u16 DeviceReset : 1;
			u16 HostProtectedArea : 1;
			u16 Obsolete1 : 1;
			u16 WriteBuffer : 1;
			u16 ReadBuffer : 1;
			u16 Nop : 1;
			u16 Obsolete2 : 1;
			u16 DownloadMicrocode : 1;
			u16 DmaQueued : 1;
			u16 Cfa : 1;
			u16 AdvancedPm : 1;
			u16 Msn : 1;
			u16 PowerUpInStandby : 1;
			u16 ManualPowerUp : 1;
			u16 Reserved2 : 1;
			u16 SetMax : 1;
			u16 Acoustics : 1;
			u16 BigLba : 1;
			u16 DeviceConfigOverlay : 1;
			u16 FlushCache : 1;
			u16 FlushCacheExt : 1;
			u16 WordValid83 : 2;
			u16 SmartErrorLog : 1;
			u16 SmartSelfTest : 1;
			u16 MediaSerialNumber : 1;
			u16 MediaCardPassThrough : 1;
			u16 StreamingFeature : 1;
			u16 GpLogging : 1;
			u16 WriteFua : 1;
			u16 WriteQueuedFua : 1;
			u16 WWN64Bit : 1;
			u16 URGReadStream : 1;
			u16 URGWriteStream : 1;
			u16 ReservedForTechReport : 2;
			u16 IdleWithUnloadFeature : 1;
			u16 WordValid : 2;
		} CommandSetSupport;
		struct {
			u16 SmartCommands : 1;
			u16 SecurityMode : 1;
			u16 RemovableMediaFeature : 1;
			u16 PowerManagement : 1;
			u16 Reserved1 : 1;
			u16 WriteCache : 1;
			u16 LookAhead : 1;
			u16 ReleaseInterrupt : 1;
			u16 ServiceInterrupt : 1;
			u16 DeviceReset : 1;
			u16 HostProtectedArea : 1;
			u16 Obsolete1 : 1;
			u16 WriteBuffer : 1;
			u16 ReadBuffer : 1;
			u16 Nop : 1;
			u16 Obsolete2 : 1;
			u16 DownloadMicrocode : 1;
			u16 DmaQueued : 1;
			u16 Cfa : 1;
			u16 AdvancedPm : 1;
			u16 Msn : 1;
			u16 PowerUpInStandby : 1;
			u16 ManualPowerUp : 1;
			u16 Reserved2 : 1;
			u16 SetMax : 1;
			u16 Acoustics : 1;
			u16 BigLba : 1;
			u16 DeviceConfigOverlay : 1;
			u16 FlushCache : 1;
			u16 FlushCacheExt : 1;
			u16 Resrved3 : 1;
			u16 Words119_120Valid : 1;
			u16 SmartErrorLog : 1;
			u16 SmartSelfTest : 1;
			u16 MediaSerialNumber : 1;
			u16 MediaCardPassThrough : 1;
			u16 StreamingFeature : 1;
			u16 GpLogging : 1;
			u16 WriteFua : 1;
			u16 WriteQueuedFua : 1;
			u16 WWN64Bit : 1;
			u16 URGReadStream : 1;
			u16 URGWriteStream : 1;
			u16 ReservedForTechReport : 2;
			u16 IdleWithUnloadFeature : 1;
			u16 Reserved4 : 2;
		} CommandSetActive;
		u16 UltraDMASupport : 8;
		u16 UltraDMAActive : 8;
		struct {
			u16 TimeRequired : 15;
			u16 ExtendedTimeReported : 1;
		} NormalSecurityEraseUnit;
		struct {
			u16 TimeRequired : 15;
			u16 ExtendedTimeReported : 1;
		} EnhancedSecurityEraseUnit;
		u16 CurrentAPMLevel : 8;
		u16 ReservedWord91 : 8;
		u16 MasterPasswordID;
		u16 HardwareResetResult;
		u16 CurrentAcousticValue : 8;
		u16 RecommendedAcousticValue : 8;
		u16 StreamMinRequestSize;
		u16 StreamingTransferTimeDMA;
		u16 StreamingAccessLatencyDMAPIO;
		u32 StreamingPerfGranularity;
		u32 Max48BitLBA[2];
		u16 StreamingTransferTime;
		u16 DsmCap;
		struct {
			u16 LogicalSectorsPerPhysicalSector : 4;
			u16 Reserved0 : 8;
			u16 LogicalSectorLongerThan256Words : 1;
			u16 MultipleLogicalSectorsPerPhysicalSector : 1;
			u16 Reserved1 : 2;
		} PhysicalLogicalSectorSize;
		u16 InterSeekDelay;
		u16 WorldWideName[4];
		u16 ReservedForWorldWideName128[4];
		u16 ReservedForTlcTechnicalReport;
		u16 WordsPerLogicalSector[2];
		struct {
			u16 ReservedForDrqTechnicalReport : 1;
			u16 WriteReadVerify : 1;
			u16 WriteUncorrectableExt : 1;
			u16 ReadWriteLogDmaExt : 1;
			u16 DownloadMicrocodeMode3 : 1;
			u16 FreefallControl : 1;
			u16 SenseDataReporting : 1;
			u16 ExtendedPowerConditions : 1;
			u16 Reserved0 : 6;
			u16 WordValid : 2;
		} CommandSetSupportExt;
		struct {
			u16 ReservedForDrqTechnicalReport : 1;
			u16 WriteReadVerify : 1;
			u16 WriteUncorrectableExt : 1;
			u16 ReadWriteLogDmaExt : 1;
			u16 DownloadMicrocodeMode3 : 1;
			u16 FreefallControl : 1;
			u16 SenseDataReporting : 1;
			u16 ExtendedPowerConditions : 1;
			u16 Reserved0 : 6;
			u16 Reserved1 : 2;
		} CommandSetActiveExt;
		u16 ReservedForExpandedSupportandActive[6];
		u16 MsnSupport : 2;
		u16 ReservedWord127 : 14;
		struct {
			u16 SecuritySupported : 1;
			u16 SecurityEnabled : 1;
			u16 SecurityLocked : 1;
			u16 SecurityFrozen : 1;
			u16 SecurityCountExpired : 1;
			u16 EnhancedSecurityEraseSupported : 1;
			u16 Reserved0 : 2;
			u16 SecurityLevel : 1;
			u16 Reserved1 : 7;
		} SecurityStatus;
		u16 ReservedWord129[31];
		struct {
			u16 MaximumCurrentInMA : 12;
			u16 CfaPowerMode1Disabled : 1;
			u16 CfaPowerMode1Required : 1;
			u16 Reserved0 : 1;
			u16 Word160Supported : 1;
		} CfaPowerMode1;
		u16 ReservedForCfaWord161[7];
		u16 NominalFormFactor : 4;
		u16 ReservedWord168 : 12;
		struct {
			u16 SupportsTrim : 1;
			u16 Reserved0 : 15;
		} DataSetManagementFeature;
		u16 AdditionalProductID[4];
		u16 ReservedForCfaWord174[2];
		u16 CurrentMediaSerialNumber[30];
		struct {
			u16 Supported : 1;
			u16 Reserved0 : 1;
			u16 WriteSameSuported : 1;
			u16 ErrorRecoveryControlSupported : 1;
			u16 FeatureControlSuported : 1;
			u16 DataTablesSuported : 1;
			u16 Reserved1 : 6;
			u16 VendorSpecific : 4;
		} SCTCommandTransport;
		u16 ReservedWord207[2];
		struct {
			u16 AlignmentOfLogicalWithinPhysical : 14;
			u16 Word209Supported : 1;
			u16 Reserved0 : 1;
		} BlockAlignment;
		u16 WriteReadVerifySectorCountMode3Only[2];
		u16 WriteReadVerifySectorCountMode2Only[2];
		struct {
			u16 NVCachePowerModeEnabled : 1;
			u16 Reserved0 : 3;
			u16 NVCacheFeatureSetEnabled : 1;
			u16 Reserved1 : 3;
			u16 NVCachePowerModeVersion : 4;
			u16 NVCacheFeatureSetVersion : 4;
		} NVCacheCapabilities;
		u16 NVCacheSizeLSW;
		u16 NVCacheSizeMSW;
		u16 NominalMediaRotationRate;
		u16 ReservedWord218;
		struct {
			u8 NVCacheEstimatedTimeToSpinUpInSeconds;
			u8 Reserved;
		} NVCacheOptions;
		u16 WriteReadVerifySectorCountMode : 8;
		u16 ReservedWord220 : 8;
		u16 ReservedWord221;
		struct {
			u16 MajorVersion : 12;
			u16 TransportType : 4;
		} TransportMajorVersion;
		u16 TransportMinorVersion;
		u16 ReservedWord224[6];
		u32 ExtendedNumberOfUserAddressableSectors[2];
		u16 MinBlocksPerDownloadMicrocodeMode03;
		u16 MaxBlocksPerDownloadMicrocodeMode03;
		u16 ReservedWord236[19];
		u16 Signature : 8;
		u16 CheckSum : 8;
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
		CMDREADDMAEX = 0x25,
		CMDWRITEDMAEX = 0x25,
		CMDIDENTIFYDEVICE = 0xEC
	};
	enum class HBAPxIS
	{
		TFES = (1 << 30)
	};
}