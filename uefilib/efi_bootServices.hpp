#pragma once
#include "efi_tables.hpp"
namespace efi
{
	struct BootServices
	{
		static constexpr uint64_t signature = 0x56524553544f4f42ull;
		TableHeader header;

		TPL (*raiseTPL)(TPL newTPL);
		void (*restoreTPL)(TPL oldTPL);

		Status (*allocatePages)(AllocateType type, MemoryType memoryType, uint64_t noPages, void* memory);
		Status (*freePages)(void* memory, uint64_t noPages);
		Status (*getMemoryMap)(uint64_t* memoryMapSize, MemoryDescriptor* memoryMap, uint64_t* mapKey, uint64_t* descriptorSize, uint32_t* descriptorVersion);
		Status (*allocatePool)(MemoryType poolType, uint64_t size, void** buffer);
		Status (*freePool)(void* buffer);

		Status (*createEvent)(EventType type, TPL notifyTPL, EventNotify notifyFunction, void* notifyContext, Event* event);
		Status (*setTimer)(Event event, TimerDelay type, uint64_t triggerTime);
		Status (*waitForEvent)(uint64_t NumberOfEvents, Event* event, uint64_t* index);
		Status (*signalEvent)(Event event);
		Status (*closeEvent)(Event event);
		Status (*checkEvent)(Event event);

		Status (*installProtocolInterface) (Handle* handle, GUID* protocol, InterfaceType interfaceType, void* interface);
		Status (*reinstallProtocolInterface) (Handle handle, GUID* protocol, void* oldInterface, void* newInterface);
		Status (*uninstallProtocolInterface) (Handle handle, GUID* protocol, void* interface);
		Status (*handleProtocol) (Handle handle, const GUID* protocol, void** interface);
		Status (*PCHandleProtocol) (Handle handle, const GUID* protocol, void** interface);
		Status (*registerProtocolNotify) (GUID* protocol, Event event, void** registration);
		Status (*locateHandle) (LocateSearchType searchType, GUID* protocol, void* serchKey, uint64_t* bufferSize, Handle* buffer);
		Status (*locateDevicePath) (GUID* protocol, DevicePath** devicePath, Handle* device);
		Status (*installConfigurationTable) (GUID* guid, void* table);

		Status(*loadImage) (bool bootPolicy, Handle parentImageHandle, DevicePath* filePath, void* sourceBuffer, uint64_t sourceSize, Handle* ImageHandle);
		Status (*startImage) (Handle imageHandle, uint64_t* exitDataSize, char16_t** exitData);
		Status (*exit) (Handle imageHandle, Status exitStatus, uint64_t exitDataSize, const char16_t* exitData);
		Status (*unloadImage) (Handle imageHandle);
		Status (*exitBootServices) (Handle imageHandle, uint64_t mapKey);

		Status (*getNextMonotonicCount) (uint64_t* count);
		Status (*stall) (uint64_t microseconds);
		Status (*setWatchdogTimer) (uint64_t timeout, uint64_t watchdogCode, uint64_t dataSize, const char16_t* watchdogData);

		Status (*connectController) (Handle controllerHandle, Handle* driverImageHandle, DevicePath* remainingDevicePath, bool recursive);
		Status (*disconnectController) (Handle controllerHandle, Handle driverImageHandle, Handle childHandle);

		Status (*openProtocol) (Handle handle, GUID* protocol, void** interface, Handle agentHandle, Handle controllerHandle, uint32_t attributes);
		Status (*closeProtocol) (Handle handle, GUID* protocol, Handle agentHandle, Handle controllerHandle);
		Status (*openProtocolInformation) (Handle agentHandle, Handle controllerHandle, uint32_t attributes, uint32_t openCount);

		Status (*protocolsPerHandle) (Handle handle, GUID*** protocolBuffer, uint64_t* protocolBufferCount);
		Status (*locateHandleBuffer) (LocateSearchType searchType, GUID* protocol, void* serchKey, uint64_t* noHandles, Handle** buffer);
		Status (*locateProtocol) (GUID* protocol, void* registration, void** interface);
		Status (*installMultipleProtocolInterfaces) (Handle* handle);
		Status (*uninstallMultipleProtocolInterfaces) (Handle* handle);

		Status (*calculateCrc32) (void* data, uint64_t dataSize, uint32_t* crc32);
		
		Status (*copyMem) (void* destination, void* source, uint64_t length);
		void (*setMem) (void* buffer, uint64_t size, uint8_t value);
		Status(*createEventEx) (uint32_t type, TPL notifyTPL, EventNotify notifyFunction, const void* notifyContext, const GUID* eventGroup, Event* event);
	};
}