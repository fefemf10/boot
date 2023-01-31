export module SMBIOS;
import types;
export import SMBIOS.structures;
export namespace SMBIOS
{
#pragma pack(1)
	union SMBIOS
	{
#pragma pack(1)
		struct 
		{
			char8_t str[4];
			u8 entryPointChecksum;
			u8 entryPointLength;
			u8 majorVersion;
			u8 minorVersion;
			u16 maxStructureSize;
			u8 entryPointRevision;
			u8 formattedArea[5];
			u8 intermediateStr[5];
			u8 intermediateChecksum;
			u16 structureTableLength;
			u32 structureTableAddress;
			u16 numberOfStructures;
			u8 BCDRevison;
		} V2;
#pragma pack(1)
		struct
		{
			char8_t str[5];
			u8 entryPointChecksum;
			u8 entryPointLength;
			u8 majorVersion;
			u8 minorVersion;
			u8 docrev;
			u8 entryPointRevision;
			u8 rsv;
			u32 structureTableMaxSize;
			u64 structureTableAddress;
		} V3;
		static SMBIOS* find()
		{
			const char8_t* rsdptr1 = u8"_SM_";
			for (size_t* i = reinterpret_cast<u64*>(0xF0000); i < reinterpret_cast<u64*>(0xFFFFF); i += 2)
			{
				for (size_t j = 0; j < 4; j++)
				{
					if (reinterpret_cast<char8_t*>(i)[j] != rsdptr1[j])
						break;
					if (j == 3)
						return reinterpret_cast<SMBIOS*>(i);
				}
			}
			const char8_t* rsdptr2 = u8"_SM3_";
			for (size_t* i = reinterpret_cast<u64*>(0xF0000); i < reinterpret_cast<u64*>(0xFFFFF); i += 2)
			{
				for (size_t j = 0; j < 5; j++)
				{
					if (reinterpret_cast<char8_t*>(i)[j] != rsdptr2[j])
						break;
					if (j == 4)
						return reinterpret_cast<SMBIOS*>(i);
				}
			}
		}
		size_t tableLength(SMBIOSHeaderStruct* header)
		{
			size_t i;
			const char* strtab = (char*)header + header->length;
			for (i = 1; strtab[i - 1] != '\0' || strtab[i] != '\0'; i++);
				return header->length + i + 1;
		}
	};
	
}