#pragma once

extern "C" unsigned char inb(unsigned short port);
extern "C" void outb(unsigned char data, unsigned short port);
extern "C" void reboot();