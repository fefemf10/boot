export module kernel;
import types;
import cpuio;
import Framebuffer;
import Font;
import console;
import memory.Descriptor;
import BootInfo;
//import IDT;
//import ISR;
//import IRQ;
//import pci;
//import memory;
//import memory.allocator;
//import sl;
//import ACPI;
//import PIT;
//import VESA;
//import driver.AHCI.structures;
//import glm;
//import glm.vec2;
//import glm.vec3;
//import glm.vec4;
//import glm.mat2x2;
//import glm.mat4x4;
//import glm.transform;
//import sl.math;
//import sl.numbers;
//import sl.vector;
//import SMBIOS;
//import memory.utils;
//import translator;

int b = 1;
void mainCRTStartup(BootInfo* bootInfo)
{
	framebuffer = bootInfo->fb;
	font = bootInfo->font;
	console::printf(u8"%x", 0x54);
	cpuio::halt();
}