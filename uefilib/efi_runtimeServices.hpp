#pragma once
#include "efi_tables.hpp"
namespace efi
{
	struct RuntimeServices
	{
		static constexpr uint64_t signature = 0x56524553544e5552ull;
		TableHeader header;

		Status (*getTime)(Time* time, TimeCapabilities* timeCapabilities);
		Status (*setTime)(Time* time);
		Status (*getWakeupTime)(bool enable, bool* pending, Time* time);
		Status (*setWakeupTime)(bool enable, Time* time);

		Status (*setVirtualAddressMap)(uint64_t memoryMapSize, uint64_t descriptorSize, uint32_t descriptorVersion, MemoryDescriptor* virtualMap);
		Status (*convertPointer)(uint64_t debugDisposition, void** address);

		Status (*getVariable)(const char16_t* variableName, GUID* vendorGUID, uint32_t* Attributes, uint64_t* dataSize, void* data);
		Status (*getNextVariableName)(uint64_t* variableNameSize, const char16_t* variableName, GUID* vendorGUID);
		Status (*setVariable)(const char16_t* variableName, GUID* vendorGUID, uint32_t Attributes, uint64_t dataSize, void* data);

		Status (*getNextHighMonotonicCount)(uint32_t* highCount);
		Status (*resetSystem)(ResetType resetType, Status ResetStatus, uint64_t dataSize, char16_t* resetData);

		void* updateCapsule;
		void* queryCapsuleCapabilities;
		void* queryVariableInfo;
	};
}