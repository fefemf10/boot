export module SMBIOS.structures;
import types;

export namespace SMBIOS
{
	enum class Type
	{
		BIOS_INFORMATION,
		SYSTEM_INFORMATION,
		BASEBOARD_INFORMATION,
		SYSTEM_ENCLOSURE,
		PROCESSOR_INFORMATION,
		MEMORY_CONTROLLER_INFORMATION,
		MEMORY_MODULE_INFORMATON,
		CACHE_INFORMATION,
		PORT_CONNECTOR_INFORMATION,
		SYSTEM_SLOTS,
		ONBOARD_DEVICE_INFORMATION,
		OEM_STRINGS,
		SYSTEM_CONFIGURATION_OPTIONS,
		BIOS_LANGUAGE_INFORMATION,
		GROUP_ASSOCIATIONS,
		SYSTEM_EVENT_LOG,
		PHYSICAL_MEMORY_ARRAY,
		MEMORY_DEVICE,
		BIT32_MEMORY_ERROR_INFORMATION,
		MEMORY_ARRAY_MAPPED_ADDRESS,
		MEMORY_DEVICE_MAPPED_ADDRESS,
		BUILT_IN_POINTING_DEVICE,
		PORTABLE_BATTERY,
		SYSTEM_RESET,
		HARDWARE_SECURITY,
		SYSTEM_POWER_CONTROLS,
		VOLTAGE_PROBE,
		COOLING_DEVICE,
		TEMPERATURE_PROBE,
		ELECTRICAL_CURRENT_PROBE,
		OUT_OF_BAND_REMOTE_ACCESS,
		BOOT_INTEGRITY_SERVICE,
		SYSTEM_BOOT_INFORMATION,
		BIT64_MEMORY_ERROR_INFORMATION,
		MANAGEMENT_DEVICE,
		MANAGEMENT_DEVICE_COMPONENT,
		MANAGEMENT_DEVICE_THRESHOLD_DATA,
		MEMORY_CHANNEL,
		IPMI_DEVICE_INFORMATION,
		SYSTEM_POWER_SUPPLY,
		ADDITIONAL_INFORMATION,
		ONBOARD_DEVICES_EXTENDED_INFORMATION,
		MANAGEMENT_CONTROLLER_HOST_INTERFACE,
		TPM_DEVICE,
		PROCESSOR_ADDITIONAL_INFORMATION,
		FIRMWARE_INVENTORY_INFORMATION,
		STRING_PROPERTY_INFORMATION,
	};
	struct SMBIOSHeaderStruct
	{
		u8 type;
		u8 length;
		u16 handle;
	};
#pragma pack(1)
	struct BIOSInformation
	{
		SMBIOSHeaderStruct header;
		u8 vendor;
		u8 version;
		u16 adressSegment;
		u8 releaseDate;
		u8 romSize;
		u64 characteristics;
		u16 characteristicsExtensionBytes;
		u8 majorRelease;
		u8 minorRelease;
		u8 embeddedControllerFirmwareMajorRelease;
		u8 embeddedControllerFirmwareMinorRelease;
		u16 extendedRomSize;
	};

	struct SystemInformation
	{
		SMBIOSHeaderStruct header;
		u8 manufacturer;
		u8 product;
		u8 version;
		u8 serialNumber;
		u8 uuid[16];
		u8 wakeupType;
		u8 SKUNumber;
		u8 family;
	};
	struct BaseboardInformation
	{
		SMBIOSHeaderStruct header;
		u8 manufacturer;
		u8 product;
		u8 version;
		u8 serialNumber;
		u8 assetTag;
		u8 featureFlags;
		u8 locationInChassis;
		u16 chassisHandle;
		u8 boardType;
		u8 numberOfContainedObjectHandles;
		u16 containedObjectHandles[1];
	};
	struct SystemEnclosure
	{
		SMBIOSHeaderStruct header;
		u8 manufacturer;
		u8 type;
		u8 version;
		u8 serialNumber;
		u8 assetTag;
		u8 bootupState;
		u8 powerSupplyState;
		u8 thermalState;
		u8 securityStatus;
		u32 OEMdefined;
		u8 height;
		u8 numberOfPowerCords;
		u8 containedElementCount;
		u8 containedElementRecordLength;
		u8 containedElements[1];
		u8 SKUNumber;
	};
	struct ProcessorInformation
	{
		SMBIOSHeaderStruct header;
		u8 socketDesignation;
		u8 processorType;
		u8 processorFamily;
		u8 processorManufacturer;
		u64 processorID;
		u8 processorVersion;
		u8 voltage;
		u16 externalClock;
		u16 maxSpeed;
		u16 currentSpeed;
		u8 status;
		u8 processorUpgrade;
		u16 l1cacheHandle;
		u16 l2cacheHandle;
		u16 l3cacheHandle;
		u8 serialNumber;
		u8 assetTag;
		u8 partNumber;
		u8 coreCount;
		u8 coreEnabled;
		u8 threadCount;
		u16 processorCharacteristics;
		u16 processorFamily2;
		u16 coreCount2;
		u16 coreEnabled2;
		u16 threadCount2;
		u16 threadEnabled;
	};
}