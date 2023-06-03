#pragma once
#include <stdint.h>
namespace efi
{
	using Handle = void*;
	using Event = void*;

	constexpr uint64_t errorBit = 0x8000000000000000ull;
	enum class Status : uint64_t
	{
		SUCCESS = 0,

		WARN_UNKNOWN_GLYPH = 1,
		WARN_DELETE_FAILURE = 2,
		WARN_WRITE_FAILURE = 3,
		WARN_BUFFER_TOO_SMALL = 4,
		WARN_STALE_DATA = 5,
		WARN_FILE_SYSTEM = 6,
		WARN_RESET_REQUIRED = 7,

		LOAD_ERROR = 1 | errorBit,
		INVALID_PARAMETER = 2 | errorBit,
		UNSUPPORTED = 3 | errorBit,
		BAD_BUFFER_SIZE = 4 | errorBit,
		BUFFER_TOO_SMALL = 5 | errorBit,
		NOT_READY = 6 | errorBit,
		DEVICE_ERROR = 7 | errorBit,
		WRITE_PROTECTED = 8 | errorBit,
		OUT_OF_RESOURCES = 9 | errorBit,
		VOLUME_CORRUPTED = 10 | errorBit,
		VOLUME_FULL = 11 | errorBit,
		NO_MEDIA = 12 | errorBit,
		MEDIA_CHANGED = 13 | errorBit,
		NOT_FOUND = 14 | errorBit,
		ACCESS_DENIED = 15 | errorBit,
		NO_RESPONSE = 16 | errorBit,
		NO_MAPPING = 17 | errorBit,
		TIMEOUT = 18 | errorBit,
		NOT_STARTED = 19 | errorBit,
		ALREADY_STARTED = 20 | errorBit,
		ABORTED = 21 | errorBit,
		ICMP_ERROR = 22 | errorBit,
		TFTP_ERROR = 23 | errorBit,
		PROTOCOL_ERROR = 24 | errorBit,
		INCOMPATIBLE_VERSION = 25 | errorBit,
		SECURITY_VIOLATION = 26 | errorBit,
		CRC_ERROR = 27 | errorBit,
		END_OF_MEDIA = 28 | errorBit,
		END_OF_FILE = 31 | errorBit,
		INVALID_LANGUAGE = 32 | errorBit,
		COMPROMISED_DATA = 33 | errorBit,
		IP_ADDRESS_CONFLICT = 34 | errorBit,
		HTTP_ERROR = 35 | errorBit
	};

	inline constexpr bool is_error(Status s) { return static_cast<uint64_t>(s) & errorBit; }
	inline constexpr bool is_warning(Status s) { return !is_error(s) && s != Status::SUCCESS; }

	struct Time
	{
		uint16_t year;
		uint8_t month;
		uint8_t day;
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
		uint8_t _pad0;
		uint32_t nanosecond;
		int16_t time_zone;
		uint8_t daylight;
		uint8_t _pad1;
	};

	struct TimeCapabilities
	{
		uint32_t resolution;
		uint32_t accuracy;
		bool setsToZero;
	};

	enum class MemoryType : uint32_t
	{
		RESERVED,
		LOADER_CODE,
		LOADER_DATA,
		BOOT_SERVICES_CODE,
		BOOT_SERVICES_DATA,
		RUNTIME_SERVICES_CODE,
		RUNTIME_SERVICES_DATA,
		CONVENTIONAL_MEMORY,
		UNUSABLE_MEMORY,
		ACPI_RECLAIM_MEMORY,
		ACPI_MEMORY_NVS,
		MEMORY_MAPPED_IO,
		MEMORY_MAPPED_IO_PORT_SPACE,
		PAL_CODE,
		PERSISTENT_MEMORY,
		MAX_MEMORY_TYPE
	};

	enum class AllocateType : uint32_t
	{
		ALLOCATE_ANY_PAGES,
		ALLOCATE_MAX_ADDRESS,
		ALLOCATE_ADDRESS,
		MAXALLOCATE_TYPE
	};

	enum class TimerDelay : uint32_t
	{
		CANCEL,
		PERIODIC,
		RELATIVE
	};

	enum class Event : uint32_t
	{
		TIMER = 0x80000000,
		RUNTIME = 0x40000000,
		NOTIFY_WAIT = 0x00000100,
		NOTIFY_SIGNAL = 0x00000200,
		SIGNAL_EXIT_BOOT_SERVICES = 0x00000201,
		SIGNAL_VIRTUAL_ADDRESS_CHANGE = 0x60000202
	};

	enum class TPL : uint64_t
	{
		APPLICATION = 4,
		CALLBACK = 8,
		NOTIFY = 16,
		HIGH_LEVEL = 31
	};

	enum class FileMode : uint64_t
	{
		FILE_MODE_READ = 0x0000000000000001,
		FILE_MODE_WRITE = 0x0000000000000002,
		FILE_MODE_CREATE = 0x8000000000000000
	};

	enum class FileAttributes : uint64_t
	{
		FILE_READ_ONLY = 0x0000000000000001,
		FILE_HIDDEN = 0x0000000000000002,
		FILE_SYSTEM = 0x0000000000000004,
		FILE_RESERVED = 0x0000000000000008,
		FILE_DIRECTORY = 0x0000000000000010,
		FILE_ARCHIVE = 0x0000000000000020,
		FILE_VALID_ATTR = 0x0000000000000037
	};

	enum class ResetType
	{
		RESET_COLD,
		RESET_WARM,
		RESET_SHUTDOWN
	};

	struct MemoryDescriptor
	{
		uint32_t type;
		uint32_t pad;
		uintptr_t physicalStart;
		uintptr_t virtualStart;
		uint64_t numberOfPages;
		uint64_t attribute;
	};

	struct InputKey
	{
		uint16_t scanCode;
		char16_t unicodeChar;
	};

	using EventNotify = void (*)(Event, void*);
}