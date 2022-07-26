#include "TeletypeVideoBuffer.hpp"
extern "C" void isr0_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Divide-by-zero Error");
	while (true);
}
extern "C" void isr1_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Debug");
	while (true);
}
extern "C" void isr2_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Non-maskable Interrupt");
	while (true);
}
extern "C" void isr3_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Debug");
	while (true);
}
extern "C" void isr4_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Overflow");
	while (true);
}
extern "C" void isr5_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Bound Range Exceeded");
	while (true);
}
extern "C" void isr6_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Invalid Opcode");
	while (true);
}
extern "C" void isr7_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Device Not Available");
	while (true);
}
extern "C" void isr8_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Double Fault");
	while (true);
}
extern "C" void isr9_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Coprocessor Segment Overrun");
	while (true);
}
extern "C" void isr10_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Invalid TSS");
	while (true);
}
extern "C" void isr11_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Segment Not Present");
	while (true);
}
extern "C" void isr12_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Stack-Segment Fault");
	while (true);
}
extern "C" void isr13_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("General Protection Fault");
	while (true);
}
extern "C" void isr14_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Page Fault");
	while (true);
}
extern "C" void isr15_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}
extern "C" void isr16_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("x87 Floating-Point Exception");
	while (true);
}
extern "C" void isr17_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Alignment Check");
	while (true);
}
extern "C" void isr18_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Machine Check");
	while (true);
}
extern "C" void isr19_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("SIMD Floationg-Point Exception");
	while (true);
}
extern "C" void isr20_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Virtualization Exception");
	while (true);
}
extern "C" void isr21_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Control Projection Exception");
	while (true);
}
extern "C" void isr22_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}
extern "C" void isr23_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}
extern "C" void isr24_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}
extern "C" void isr25_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}
extern "C" void isr26_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}
extern "C" void isr27_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}
extern "C" void isr28_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Hypervisior Injection Exception");
	while (true);
}
extern "C" void isr29_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("VMM Communication Exception");
	while (true);
}
extern "C" void isr30_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Security Exception");
	while (true);
}
extern "C" void isr31_handler()
{
	TeletypeVideoBuffer::clear();
	TeletypeVideoBuffer::printf("Reserved");
	while (true);
}