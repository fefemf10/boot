#pragma once
#include "efi_types.hpp"
#include "efi_GUID.hpp"
namespace efi
{
	struct RuntimeServices;
	struct BootServices;
	struct SimpleTextInput;
	struct SimpleTextOutput;
	struct TableHeader
	{
		uint64_t signature;
		uint32_t revision;
		uint32_t headerSize;
		uint32_t crc32;
		uint32_t reserved;
	};
	struct ConfigurationTable
	{
		GUID vendorGUID;
		void* vendorTable;
	};
	struct SystemTable
	{
		TableHeader header;
		char16_t* firmwareVendor;
		uint32_t firmwareRevision;
		Handle conInHandle;
		SimpleTextInput* conIn;
		Handle conOutHandle;
		SimpleTextOutput* conOut;
		Handle stdErrorHandle;
		SimpleTextOutput* stdError;
		RuntimeServices* runtimeServices;
		BootServices* bootServices;
		uint32_t numberOfTableEntries;
		ConfigurationTable* configurationTables;
	};
	constexpr inline uint32_t makeSystemTableRevision(int32_t major, int32_t minor)
	{
		return (major << 16) | minor;
	}
	constexpr uint64_t SystemTableSignature = 0x5453595320494249ull;
	constexpr uint32_t SystemTableRevision = makeSystemTableRevision(2, 70);
	constexpr uint32_t SpecificationRevision = SystemTableRevision;
	namespace vendorGUID
	{
		constexpr GUID acpi1{ 0xeb9d2d30,0x2d88,0x11d3,{0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d} };
		constexpr GUID acpi1{ 0x8868e871,0xe4f1,0x11d3,{0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81} };
	}
}