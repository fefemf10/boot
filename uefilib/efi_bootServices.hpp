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

		Status (*eventNotify)(Event event, void* Context);
		Status (*createEvent)(Event type, TPL notifyTPL, EventNotify notifyFunction, void* notifyContext, Event* event);
		Status (*setTimer)(Event event, TimerDelay type, uint64_t triggerTime);
		Status (*signalEvent)(Event event);
		Status (*waitForEvent)(uint64_t NumberOfEvents, Event* event, uint64_t* index);
		Status (*closeEvent)(Event event);
		Status (*checkEvent)(Event event);
	};
}