export module PE;
import types;
export namespace PE
{
	struct AuxSymbol5
	{
		u32 length;
		u16 numberRelocation;
		u16 numberLineNumbers;
		u32 checkSum;
		u16 number;
		u8 selection;
		u8 unused[3];
	};

	struct AuxSymbol4
	{
		u8 fileName[18];
	};

	struct AuxSymbol3
	{
		u32 tagIndex;
		u32 characteristics;
		u8 unused[10];
	};

	struct AuxSymbol2
	{
		u32 unused;
		u16 numberLineNumbers;
		u8 unused2[6];
		u32 pointerNextFunction;
		u8 unused3[2];
	};

	struct AuxSymbol
	{
		u32 tagIndex;
		u32 totalSize;
		u32 pointerLineNumber;
		u32 pointerNextFunction;
		u8 unused[2];
	};

	struct Symbol
	{
		enum class Type : u16
		{
			IMAGE_SYM_DTYPE_NULL = 0x0,
			IMAGE_SYM_DTYPE_POINTER = 0x100,
			IMAGE_SYM_DTYPE_FUNCTION = 0x20,
			IMAGE_SYM_DTYPE_ARRAY = 0x300
		};
		enum class StorageClass : u8
		{
			IMAGE_SYM_CLASS_END_OF_FUNCTION = 0xFF,
			IMAGE_SYM_CLASS_NULL = 0x0,
			IMAGE_SYM_CLASS_AUTOMATIC = 0x1,
			IMAGE_SYM_CLASS_EXTERNAL = 0x2,
			IMAGE_SYM_CLASS_STATIC = 0x3,
			IMAGE_SYM_CLASS_REGISTER = 0x4,
			IMAGE_SYM_CLASS_EXTERNAL_DEF = 0x5,
			IMAGE_SYM_CLASS_LABEL = 0x6,
			IMAGE_SYM_CLASS_UNDEFINED_LABEL = 0x7,
			IMAGE_SYM_CLASS_MEMBER_OF_STRUCT = 0x8,
			IMAGE_SYM_CLASS_ARGUMENT = 0x9,
			IMAGE_SYM_CLASS_STRUCT_TAG = 0xA,
			IMAGE_SYM_CLASS_MEMBER_OF_UNION = 0xB,
			IMAGE_SYM_CLASS_UNION_TAG = 0xC,
			IMAGE_SYM_CLASS_TYPE_DEFINITION = 0xD,
			IMAGE_SYM_CLASS_UNDEFINED_STATIC = 0xE,
			IMAGE_SYM_CLASS_ENUM_TAG = 0xF,
			IMAGE_SYM_CLASS_MEMBER_OF_ENUM = 0x10,
			IMAGE_SYM_CLASS_REGISTER_PARAM = 0x11,
			IMAGE_SYM_CLASS_BIT_FIELD = 0x12,
			IMAGE_SYM_CLASS_BLOCK = 0x64,
			IMAGE_SYM_CLASS_FUNCTION = 0x65,
			IMAGE_SYM_CLASS_END_OF_STRUCT = 0x66,
			IMAGE_SYM_CLASS_FILE = 0x67,
			IMAGE_SYM_CLASS_SECTION = 0x68,
			IMAGE_SYM_CLASS_WEAK_EXTERNAL = 0x69,
			IMAGE_SYM_CLASS_CLR_TOKEN = 0x6B
		};
		const char8_t* name;
		u32 value;
		u16 sectionNumber;
		Type type;
		StorageClass storageClass;
		u8 numberAuxSymbols;
	};

	struct Relocation
	{
		enum class Type : u16
		{
			IMAGE_REL_AMD64_ABSOLUTE = 0x0,
			IMAGE_REL_AMD64_ADDR64 = 0x1,
			IMAGE_REL_AMD64_ADDR32 = 0x2,
			IMAGE_REL_AMD64_ADDR32NB = 0x3,
			IMAGE_REL_AMD64_REL32 = 0x4,
			IMAGE_REL_AMD64_REL32_1 = 0x5,
			IMAGE_REL_AMD64_REL32_2 = 0x6,
			IMAGE_REL_AMD64_REL32_3 = 0x7,
			IMAGE_REL_AMD64_REL32_4 = 0x8,
			IMAGE_REL_AMD64_REL32_5 = 0x9,
			IMAGE_REL_AMD64_SECTION = 0xA,
			IMAGE_REL_AMD64_SECREL = 0xB,
			IMAGE_REL_AMD64_SECREL7 = 0xC,
			IMAGE_REL_AMD64_TOKEN = 0xD,
			IMAGE_REL_AMD64_SREL32 = 0xE,
			IMAGE_REL_AMD64_PAIR = 0xF,
			IMAGE_REL_AMD64_SSPAN32 = 0x10
		};
		u32 virtualAddress;
		u32 symbolTableIndex;
		Type type;
	};

	struct Section
	{
		enum class Flags : u32
		{
			IMAGE_SCN_TYPE_NO_PAD = 0x8,
			IMAGE_SCN_CNT_CODE = 0x20,
			IMAGE_SCN_CNT_INITIALIZED_DATA = 0x40,
			IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x80,
			IMAGE_SCN_LNK_OTHER = 0x100,
			IMAGE_SCN_LNK_INFO = 0x200,
			IMAGE_SCN_LNK_REMOVE = 0x800,
			IMAGE_SCN_LNK_COMDAT = 0x1000,
			IMAGE_SCN_GPREL = 0x8000,
			IMAGE_SCN_MEM_PURGEABLE = 0x20000,
			IMAGE_SCN_MEM_16BIT = 0x20000,
			IMAGE_SCN_MEM_LOCKED = 0x40000,
			IMAGE_SCN_MEM_PRELOAD = 0x80000,
			IMAGE_SCN_ALIGN_1BYTES = 0x100000,
			IMAGE_SCN_ALIGN_2BYTES = 0x200000,
			IMAGE_SCN_ALIGN_4BYTES = 0x300000,
			IMAGE_SCN_ALIGN_8BYTES = 0x400000,
			IMAGE_SCN_ALIGN_16BYTES = 0x500000,
			IMAGE_SCN_ALIGN_32BYTES = 0x600000,
			IMAGE_SCN_ALIGN_64BYTES = 0x700000,
			IMAGE_SCN_ALIGN_128BYTES = 0x800000,
			IMAGE_SCN_ALIGN_256BYTES = 0x900000,
			IMAGE_SCN_ALIGN_512BYTES = 0xA00000,
			IMAGE_SCN_ALIGN_1024BYTES = 0xB00000,
			IMAGE_SCN_ALIGN_2048BYTES = 0xC00000,
			IMAGE_SCN_ALIGN_4096BYTES = 0xD00000,
			IMAGE_SCN_ALIGN_8192BYTES = 0xE00000,
			IMAGE_SCN_LNK_NRELOC_OVFL = 0x1000000,
			IMAGE_SCN_MEM_DISCARDABLE = 0x2000000,
			IMAGE_SCN_MEM_NOT_CACHED = 0x4000000,
			IMAGE_SCN_MEM_NOT_PAGED = 0x8000000,
			IMAGE_SCN_MEM_SHARED = 0x10000000,
			IMAGE_SCN_MEM_EXECUTE = 0x20000000,
			IMAGE_SCN_MEM_READ = 0x40000000,
			IMAGE_SCN_MEM_WRITE = 0x80000000
		};
		const char8_t name[8]{};
		u32 virtualSize;
		u32 virtualAddress;
		u32 sizeRawData;
		u32 pointerRawData;
		u32 pointerRelocationTable;
		u32 pointerLineNumbers;
		u16 numberRelocations;
		u16 numberLineNumbers;
		Flags flags;
	};

	enum class Subsystem : u16
	{
		IMAGE_SUBSYSTEM_UNKNOWN = 0,
		IMAGE_SUBSYSTEM_NATIVE = 1,
		IMAGE_SUBSYSTEM_WINDOWS_GUI = 2,
		IMAGE_SUBSYSTEM_WINDOWS_CUI = 3,
		IMAGE_SUBSYSTEM_OS2_CUI = 5,
		IMAGE_SUBSYSTEM_POSIX_CUI = 7,
		IMAGE_SUBSYSTEM_NATIVE_WINDOWS = 8,
		IMAGE_SUBSYSTEM_WINDOWS_CE_GUI = 9,
		IMAGE_SUBSYSTEM_EFI_APPLICATION = 10,
		IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11,
		IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER = 12,
		IMAGE_SUBSYSTEM_EFI_ROM = 13,
		IMAGE_SUBSYSTEM_XBOX = 14,
		IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION = 16,
	};
	
	enum class Characteristics : u16
	{
		IMAGE_FILE_RELOCS_STRIPPED = 0x0001,
		IMAGE_FILE_EXECUTABLE_IMAGE = 0x0002,
		IMAGE_FILE_LINE_NUMS_STRIPPED = 0x0004,
		IMAGE_FILE_LOCAL_SYMS_STRIPPED = 0x0008,
		IMAGE_FILE_AGGRESSIVE_WS_TRIM = 0x0010,
		IMAGE_FILE_LARGE_ADDRESS_AWARE = 0x0020,
		IMAGE_FILE_BYTES_REVERSED_LO = 0x0080,
		IMAGE_FILE_32BIT_MACHINE = 0x0100,
		IMAGE_FILE_DEBUG_STRIPPED = 0x0200,
		IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP = 0x0400,
		IMAGE_FILE_NET_RUN_FROM_SWAP = 0x0800,
		IMAGE_FILE_SYSTEM = 0x1000,
		IMAGE_FILE_DLL = 0x2000,
		IMAGE_FILE_UP_SYSTEM_ONLY = 0x4000,
		IMAGE_FILE_BYTES_REVERSED_HI = 0x8000,
	};
	struct DataDirectory
	{
		u32 virtualAddress;
		u32 size;
	};
#pragma pack(1)
	struct OptionalHeaderData
	{
		DataDirectory exportTable;
		DataDirectory importTable;
		DataDirectory resourceTable;
		DataDirectory exceptionTable;
		DataDirectory certificateTable;
		DataDirectory baseRelocationTable;
		DataDirectory debug;
		DataDirectory architecture;
		DataDirectory globalPtr;
		DataDirectory TLSTable;
		DataDirectory loadConfigTable;
		DataDirectory boundImport;
		DataDirectory importAddressTable;
		DataDirectory delayImportDescriptor;
		DataDirectory CLRRuntimeHeader;
		DataDirectory rsv;
	};

#pragma pack(1)
	struct OptionalHeader
	{
		u16 magic;
		u8 majorLinkerVersion;
		u8 minorLinkerVersion;
		u32 sizeOfCode;
		u32 sizeOfInitializedData;
		u32 sizeOfUninitializedData;
		u32 addressOfEntryPoint;
		u32 baseOfCode;
		u64 imageBase;
		u32 sectionAlignment;
		u32 fileAlignment;
		u16 majorOperatingSystemVersion;
		u16 minorOperatingSystemVersion;
		u16 majorImageVersion;
		u16 minorImageVersion;
		u16 majorSubsystemVersion;
		u16 minorSubsystemVersion;
		u32 win32VersionValue;
		u32 sizeOfImage;
		u32 sizeOfHeaders;
		u32 checksum;
		Subsystem subsystem;
		u16 dllCharacteristics;
		u64 sizeOfStackReserve;
		u64 sizeOfStackCommit;
		u64 sizeOfHeapReserve;
		u64 sizeOfHeapCommit;
		u32 loaderFlags;
		u32 numberOfRvaAndSizes;
		constexpr bool isValid() const
		{
			return magic == 0x020B;
		}
	};

#pragma pack(1)
	struct COFF
	{
		u32 signature;
		u16 machine;
		u16 numberSection;
		u32 timeDateStamp;
		u32 pointerSymbolTable;
		u32 numberSymbol;
		u16 sizeOptionalHeader;
		Characteristics characteristics;
		constexpr bool isValid() const
		{
			return signature == 0x4550 && machine == 0x8664 && ((u16)characteristics & (u16)Characteristics::IMAGE_FILE_EXECUTABLE_IMAGE);
		}
	};

#pragma pack(1)
	struct MSDOSHeader
	{
		u16 magic;
		u16 cblp;
		u16 cp;
		u16 crlc;
		u16 cparhdr;
		u16 minalloc;
		u16 maxalloc;
		u16 ss;
		u16 sp;
		u16 csum;
		u16 ip;
		u16 cs;
		u16 lfarlc;
		u16 ovno;
		u16 res[4];
		u16 oemid;
		u16 oeminfo;
		u16 res2[10];
		u32 lfanew;
		constexpr bool isValid() const
		{
			return magic == 0x5A4D;
		}
	};
#pragma pack(1)
	struct PE
	{
		MSDOSHeader msdosHeader;
		COFF coffHeader;
		OptionalHeader optionalHeader;
		OptionalHeaderData optionalHeaderData;
		constexpr bool isValid() const
		{
			return msdosHeader.isValid() && coffHeader.isValid() && optionalHeader.isValid();
		}
	};
}