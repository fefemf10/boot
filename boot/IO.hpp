#pragma once

extern "C" unsigned char inb(unsigned short port);
extern "C" unsigned short inw(unsigned short port);
extern "C" void outb(unsigned char data, unsigned short port);
extern "C" void outw(unsigned short data, unsigned short port);
extern "C" void reboot();
extern "C" void iowait();