#pragma once
#include <cstdint>
#include "IO.hpp"
#include "TeletypeVideoBuffer.hpp"

struct IDT64
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_attr;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t zero;
};
extern "C" IDT64 _idt[256]{};
extern "C" uint64_t isr1;
extern "C" void loadIDT();

void initializeIDT()
{
	for (uint64_t i = 0; i < 256; i++)
	{
		_idt[i].offset_low = (uint16_t)(((uint64_t)&isr1 & 0x000000000000ffff));
		_idt[i].selector = 0x08;
		_idt[i].ist = 0x0;
		_idt[i].type_attr = 0x8e;
		_idt[i].offset_mid = (uint16_t)(((uint64_t)&isr1 & 0x00000000ffff0000) >> 16u);
		_idt[i].offset_high = (uint32_t)(((uint64_t)&isr1 & 0xffffffff00000000) >> 32u);
		_idt[i].zero = 0;
	}
	outb(0xfd,0x21);
	outb(0xff,0xa1);
	loadIDT();
}

extern "C" void isr1_handler()
{
	TeletypeVideoBuffer::puts(TeletypeVideoBuffer::hexToString(inb(0x60)));
	outb(0x20, 0x20);
	outb(0x20, 0xa0);
}