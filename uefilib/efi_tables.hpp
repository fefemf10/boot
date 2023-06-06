#pragma once
#include "efi_types.hpp"
#include "efi_GUID.hpp"
namespace efi
{
	struct RuntimeServices;
	struct BootServices;
	struct SimpleTextInputInterface;
	struct SimpleTextOutputInterface;
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
		SimpleTextInputInterface* conIn;
		Handle conOutHandle;
		SimpleTextOutputInterface* conOut;
		Handle stdErrorHandle;
		SimpleTextOutputInterface* stdError;
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
}