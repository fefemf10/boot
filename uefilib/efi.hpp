#pragma once
using uint64_t = unsigned __int64;
using int64_t = __int64;
using uint32_t = unsigned __int32;
using int32_t = __int32;
using uint16_t = unsigned __int16;
using int16_t = __int16;
using uint8_t = unsigned __int8;
using int8_t = __int8;

using CHAR16 = wchar_t;
using UINT64 = uint64_t;
using INT64 = int64_t;
using UINT32 = uint32_t;
using INT32 = int32_t;
using UINT16 = uint16_t;
using INT16 = int16_t;
using UINT8 = uint8_t;
using CHAR8 = char;
using INT8 = int8_t;
using BOOLEAN = int8_t;

using VOID = void;
using INTN = int64_t;
using UINTN = uint64_t;

using EFI_STATUS = UINTN;
using EFI_LBA = UINT64;
using EFI_TPL = UINTN;
using EFI_HANDLE = void*;
using EFI_EVENT = void*;

struct EFI_TABLE_HEADER
{
	UINT64                      Signature;
	UINT32                      Revision;
	UINT32                      HeaderSize;
	UINT32                      CRC32;
	UINT32                      Reserved;
};
struct EFI_INPUT_KEY
{
	UINT16                              ScanCode;
	CHAR16                              UnicodeChar;
};

struct SIMPLE_INPUT_INTERFACE;
using EFI_INPUT_RESET = EFI_STATUS(*) (SIMPLE_INPUT_INTERFACE* This, BOOLEAN ExtendedVerification);
using EFI_INPUT_READ_KEY = EFI_STATUS(*) (SIMPLE_INPUT_INTERFACE* This, EFI_INPUT_KEY* Key);

struct SIMPLE_INPUT_INTERFACE
{
	EFI_INPUT_RESET                     Reset;
	EFI_INPUT_READ_KEY                  ReadKeyStroke;
	EFI_EVENT                           WaitForKey;
};

using EFI_SIMPLE_TEXT_IN_PROTOCOL = SIMPLE_INPUT_INTERFACE;
struct SIMPLE_TEXT_OUTPUT_INTERFACE;

using EFI_TEXT_CLEAR_SCREEN = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This);
using EFI_TEXT_SET_CURSOR_POSITION = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN Column, UINTN Row);
using EFI_TEXT_ENABLE_CURSOR = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, BOOLEAN Enable);

struct SIMPLE_TEXT_OUTPUT_MODE
{
	INT32                           MaxMode;
	// current settings
	INT32                           Mode;
	INT32                           Attribute;
	INT32                           CursorColumn;
	INT32                           CursorRow;
	BOOLEAN                         CursorVisible;
};

using EFI_TEXT_RESET = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, BOOLEAN ExtendedVerification);
using EFI_TEXT_OUTPUT_STRING = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, const CHAR16* String);
using EFI_TEXT_TEST_STRING = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, CHAR16* String);
using EFI_TEXT_QUERY_MODE = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN ModeNumber, UINTN* Columns, UINTN* Rows);
using EFI_TEXT_SET_MODE = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN ModeNumber);
using EFI_TEXT_SET_ATTRIBUTE = EFI_STATUS(*) (SIMPLE_TEXT_OUTPUT_INTERFACE* This, UINTN Attribute);


struct SIMPLE_TEXT_OUTPUT_INTERFACE
{
	EFI_TEXT_RESET                  Reset;

	EFI_TEXT_OUTPUT_STRING          OutputString;
	EFI_TEXT_TEST_STRING            TestString;

	EFI_TEXT_QUERY_MODE             QueryMode;
	EFI_TEXT_SET_MODE               SetMode;
	EFI_TEXT_SET_ATTRIBUTE          SetAttribute;

	EFI_TEXT_CLEAR_SCREEN           ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION    SetCursorPosition;
	EFI_TEXT_ENABLE_CURSOR          EnableCursor;

	// Current mode
	SIMPLE_TEXT_OUTPUT_MODE* Mode;
};
using EFI_SIMPLE_TEXT_OUT_PROTOCOL = SIMPLE_TEXT_OUTPUT_INTERFACE;

struct EFI_TIME
{
	UINT16      Year;       // 1998 - 20XX
	UINT8       Month;      // 1 - 12
	UINT8       Day;        // 1 - 31
	UINT8       Hour;       // 0 - 23
	UINT8       Minute;     // 0 - 59
	UINT8       Second;     // 0 - 59
	UINT8       Pad1;
	UINT32      Nanosecond; // 0 - 999,999,999
	INT16       TimeZone;   // -1440 to 1440 or 2047
	UINT8       Daylight;
	UINT8       Pad2;
};

struct EFI_TIME_CAPABILITIES
{
	UINT32                      Resolution;     // 1e-6 parts per million
	UINT32                      Accuracy;       // hertz
	BOOLEAN                     SetsToZero;     // Set clears sub-second time
};

using EFI_GET_TIME = EFI_STATUS(*) (EFI_TIME* Time, EFI_TIME_CAPABILITIES* Capabilities);
using EFI_SET_TIME = EFI_STATUS(*) (EFI_TIME* Time);
using EFI_GET_WAKEUP_TIME = EFI_STATUS(*) (BOOLEAN* Enabled, BOOLEAN* Pending, EFI_TIME* Time);
using EFI_SET_WAKEUP_TIME = EFI_STATUS(*) (BOOLEAN Enable, EFI_TIME* Time);
using EFI_PHYSICAL_ADDRESS = UINT64;
using EFI_VIRTUAL_ADDRESS = UINT64;
struct EFI_MEMORY_DESCRIPTOR
{
	UINT32                          Type;           // Field size is 32 bits followed by 32 bit pad
	UINT32                          Pad;
	EFI_PHYSICAL_ADDRESS            PhysicalStart;  // Field size is 64 bits
	EFI_VIRTUAL_ADDRESS             VirtualStart;   // Field size is 64 bits
	UINT64                          NumberOfPages;  // Field size is 64 bits
	UINT64                          Attribute;      // Field size is 64 bits
};
using EFI_SET_VIRTUAL_ADDRESS_MAP = EFI_STATUS(*) (UINTN MemoryMapSize, UINTN DescriptorSize, UINT32 DescriptorVersion, EFI_MEMORY_DESCRIPTOR* VirtualMap);
using EFI_CONVERT_POINTER = EFI_STATUS(*) (UINTN DebugDisposition, VOID** Address);

struct EFI_GUID
{
	UINT32  Data1;
	UINT16  Data2;
	UINT16  Data3;
	UINT8   Data4[8];
};

using EFI_GET_VARIABLE = EFI_STATUS(*) (CHAR16* VariableName, EFI_GUID* VendorGuid, UINT32* Attributes, UINTN* DataSize, VOID* Data);
using EFI_GET_NEXT_VARIABLE_NAME = EFI_STATUS(*) (UINTN* VariableNameSize, CHAR16* VariableName, EFI_GUID* VendorGuid);
using EFI_SET_VARIABLE = EFI_STATUS(*) (CHAR16* VariableName, EFI_GUID* VendorGuid, UINT32 Attributes, UINTN DataSize, VOID* Data);

using EFI_GET_NEXT_HIGH_MONO_COUNT = EFI_STATUS(*) (UINT32* HighCount);

enum EFI_RESET_TYPE
{
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown
};

using EFI_RESET_SYSTEM = EFI_STATUS(*) (EFI_RESET_TYPE ResetType, EFI_STATUS ResetStatus, UINTN DataSize, CHAR16* ResetData);

struct EFI_RUNTIME_SERVICES
{
	EFI_TABLE_HEADER                Hdr;

	//
	// Time services
	//

	EFI_GET_TIME                    GetTime;
	EFI_SET_TIME                    SetTime;
	EFI_GET_WAKEUP_TIME             GetWakeupTime;
	EFI_SET_WAKEUP_TIME             SetWakeupTime;

	//
	// Virtual memory services
	//

	EFI_SET_VIRTUAL_ADDRESS_MAP     SetVirtualAddressMap;
	EFI_CONVERT_POINTER             ConvertPointer;

	//
	// Variable serviers
	//

	EFI_GET_VARIABLE                GetVariable;
	EFI_GET_NEXT_VARIABLE_NAME      GetNextVariableName;
	EFI_SET_VARIABLE                SetVariable;

	//
	// Misc
	//

	EFI_GET_NEXT_HIGH_MONO_COUNT    GetNextHighMonotonicCount;
	EFI_RESET_SYSTEM                ResetSystem;

	/*EFI_UPDATE_CAPSULE              UpdateCapsule;
	EFI_QUERY_CAPSULE_CAPABILITIES  QueryCapsuleCapabilities;
	EFI_QUERY_VARIABLE_INFO         QueryVariableInfo;*/
};
using EFI_RAISE_TPL = EFI_TPL(*) (EFI_TPL NewTpl);
using EFI_RESTORE_TPL = VOID(*) (EFI_TPL OldTpl);

enum EFI_ALLOCATE_TYPE
{
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddress,
	MaxAllocateType
};

enum EFI_MEMORY_TYPE
{
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,
	EfiBootServicesData,
	EfiRuntimeServicesCode,
	EfiRuntimeServicesData,
	EfiConventionalMemory,
	EfiUnusableMemory,
	EfiACPIReclaimMemory,
	EfiACPIMemoryNVS,
	EfiMemoryMappedIO,
	EfiMemoryMappedIOPortSpace,
	EfiPalCode,
	EfiMaxMemoryType
};

using EFI_ALLOCATE_PAGES = EFI_STATUS(*) (EFI_ALLOCATE_TYPE Type, EFI_MEMORY_TYPE MemoryType, UINTN NoPages, EFI_PHYSICAL_ADDRESS* Memory);
using EFI_FREE_PAGES = EFI_STATUS(*) (EFI_PHYSICAL_ADDRESS Memory, UINTN NoPages);
using EFI_GET_MEMORY_MAP = EFI_STATUS(*) (UINTN* MemoryMapSize, EFI_MEMORY_DESCRIPTOR* MemoryMap, UINTN* MapKey, UINTN* DescriptorSize, UINT32* DescriptorVersion);
using EFI_ALLOCATE_POOL = EFI_STATUS(*) (EFI_MEMORY_TYPE PoolType, UINTN Size, VOID** Buffer);
using EFI_FREE_POOL = EFI_STATUS(*) (VOID* Buffer);

enum EFI_TIMER_DELAY
{
	TimerCancel,
	TimerPeriodic,
	TimerRelative,
	TimerTypeMax
};

using EFI_EVENT_NOTIFY = VOID(*) (EFI_EVENT Event, VOID* Context);
using EFI_CREATE_EVENT = EFI_STATUS(*) (UINT32 Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY NotifyFunction, VOID* NotifyContext, EFI_EVENT* Event);
using EFI_SET_TIMER = EFI_STATUS(*) (EFI_EVENT Event, EFI_TIMER_DELAY Type, UINT64 TriggerTime);
using EFI_SIGNAL_EVENT = EFI_STATUS(*) (EFI_EVENT Event);
using EFI_WAIT_FOR_EVENT = EFI_STATUS(*) (UINTN NumberOfEvents, EFI_EVENT* Event, UINTN* Index);
using EFI_CLOSE_EVENT = EFI_STATUS(*) (EFI_EVENT Event);
using EFI_CHECK_EVENT = EFI_STATUS(*) (EFI_EVENT Event);


struct EFI_BOOT_SERVICES
{

	EFI_TABLE_HEADER                Hdr;

	//
	// Task priority functions
	//

	EFI_RAISE_TPL                   RaiseTPL;
	EFI_RESTORE_TPL                 RestoreTPL;

	//
	// Memory functions
	//

	EFI_ALLOCATE_PAGES              AllocatePages;
	EFI_FREE_PAGES                  FreePages;
	EFI_GET_MEMORY_MAP              GetMemoryMap;
	EFI_ALLOCATE_POOL               AllocatePool;
	EFI_FREE_POOL                   FreePool;

	//
	// Event & timer functions
	//

	EFI_CREATE_EVENT                CreateEvent;
	EFI_SET_TIMER                   SetTimer;
	EFI_WAIT_FOR_EVENT              WaitForEvent;
	EFI_SIGNAL_EVENT                SignalEvent;
	EFI_CLOSE_EVENT                 CloseEvent;
	EFI_CHECK_EVENT                 CheckEvent;

	//
	// Protocol handler functions
	//

	//EFI_INSTALL_PROTOCOL_INTERFACE  InstallProtocolInterface;
	//EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
	//EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;
	//EFI_HANDLE_PROTOCOL             HandleProtocol;
	//EFI_HANDLE_PROTOCOL             PCHandleProtocol;
	//EFI_REGISTER_PROTOCOL_NOTIFY    RegisterProtocolNotify;
	//EFI_LOCATE_HANDLE               LocateHandle;
	//EFI_LOCATE_DEVICE_PATH          LocateDevicePath;
	//EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable;

	////
	//// Image functions
	////

	//EFI_IMAGE_LOAD                  LoadImage;
	//EFI_IMAGE_START                 StartImage;
	//EFI_EXIT                        Exit;
	//EFI_IMAGE_UNLOAD                UnloadImage;
	//EFI_EXIT_BOOT_SERVICES          ExitBootServices;

	////
	//// Misc functions
	////

	//EFI_GET_NEXT_MONOTONIC_COUNT    GetNextMonotonicCount;
	//EFI_STALL                       Stall;
	//EFI_SET_WATCHDOG_TIMER          SetWatchdogTimer;

	////
	//// DriverSupport Services
	////

	//EFI_CONNECT_CONTROLLER          ConnectController;
	//EFI_DISCONNECT_CONTROLLER       DisconnectController;

	////
	//// Open and Close Protocol Services
	////
	//EFI_OPEN_PROTOCOL               OpenProtocol;
	//EFI_CLOSE_PROTOCOL              CloseProtocol;
	//EFI_OPEN_PROTOCOL_INFORMATION   OpenProtocolInformation;

	////
	//// Library Services
	////
	//EFI_PROTOCOLS_PER_HANDLE        ProtocolsPerHandle;
	//EFI_LOCATE_HANDLE_BUFFER        LocateHandleBuffer;
	//EFI_LOCATE_PROTOCOL             LocateProtocol;
	//EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES InstallMultipleProtocolInterfaces;
	//EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;

	////
	//// 32-bit CRC Services
	////
	//EFI_CALCULATE_CRC32             CalculateCrc32;

	////
	//// Misc Services
	////
	//EFI_COPY_MEM                    CopyMem;
	//EFI_SET_MEM                     SetMem;
	//EFI_CREATE_EVENT_EX             CreateEventEx;
};

struct EFI_SYSTEM_TABLE
{
	EFI_TABLE_HEADER                Hdr;

	CHAR16* FirmwareVendor;
	UINT32                          FirmwareRevision;

	EFI_HANDLE                      ConsoleInHandle;
	SIMPLE_INPUT_INTERFACE* ConIn;

	EFI_HANDLE                      ConsoleOutHandle;
	SIMPLE_TEXT_OUTPUT_INTERFACE* ConOut;

	EFI_HANDLE                      StandardErrorHandle;
	SIMPLE_TEXT_OUTPUT_INTERFACE* StdErr;

	EFI_RUNTIME_SERVICES* RuntimeServices;
	EFI_BOOT_SERVICES* BootServices;

	/*UINTN                           NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE* ConfigurationTable;*/

};