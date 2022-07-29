export module ISR;
import types;
import IDT;
import cpuio;
import teletype;
import PIC;
namespace ISR
{
	extern "C" void isr0();
	extern "C" void isr1();
	extern "C" void isr2();
	extern "C" void isr3();
	extern "C" void isr4();
	extern "C" void isr5();
	extern "C" void isr6();
	extern "C" void isr7();
	extern "C" void isr8();
	extern "C" void isr9();
	extern "C" void isr10();
	extern "C" void isr11();
	extern "C" void isr12();
	extern "C" void isr13();
	extern "C" void isr14();
	extern "C" void isr15();
	extern "C" void isr16();
	extern "C" void isr17();
	extern "C" void isr18();
	extern "C" void isr19();
	extern "C" void isr20();
	extern "C" void isr21();
	extern "C" void isr22();
	extern "C" void isr23();
	extern "C" void isr24();
	extern "C" void isr25();
	extern "C" void isr26();
	extern "C" void isr27();
	extern "C" void isr28();
	extern "C" void isr29();
	extern "C" void isr30();
	extern "C" void isr31();
	const char* errors[32]
	{
		"Divide-by-zero Error",
		"Debug",
		"Non-maskable Interrupt",
		"Debug",
		"Overflow",
		"Bound Range Exceeded",
		"Invalid Opcode",
		"Device Not Available",
		"Double Fault",
		"Coprocessor Segment Overrun",
		"Invalid TSS",
		"Segment Not Present",
		"Stack-Segment Fault",
		"General Protection Fault",
		"Page Fault",
		"Reserved",
		"x87 Floating-Point Exception",
		"Alignment Check",
		"Machine Check",
		"SIMD Floationg-Point Exception",
		"Virtualization Exception",
		"Control Projection Exception",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Hypervisior Injection Exception",
		"VMM Communication Exception",
		"Security Exception",
		"Reserved"
	};
	extern "C" void isrHandler(const cpuio::regs& regs)
	{
		teletype::putregs(regs);
		teletype::puth(regs.interruptCode);
		teletype::puth(errors);
		teletype::puth(errors[0]);
		//teletype::puts(errors[regs.interruptCode]);
		PIC::eio();
		while (true);
		
	}
	export void initialize()
	{
		IDT::set(0, isr0);
		IDT::set(1, isr1);
		IDT::set(2, isr2);
		IDT::set(3, isr3);
		IDT::set(4, isr4);
		IDT::set(5, isr5);
		IDT::set(6, isr6);
		IDT::set(7, isr7);
		IDT::set(8, isr8);
		IDT::set(9, isr9);
		IDT::set(10, isr10);
		IDT::set(11, isr11);
		IDT::set(12, isr12);
		IDT::set(13, isr13);
		IDT::set(14, isr14);
		IDT::set(15, isr15);
		IDT::set(16, isr16);
		IDT::set(17, isr17);
		IDT::set(18, isr18);
		IDT::set(19, isr19);
		IDT::set(20, isr20);
		IDT::set(21, isr21);
		IDT::set(22, isr22);
		IDT::set(23, isr23);
		IDT::set(24, isr24);
		IDT::set(25, isr25);
		IDT::set(26, isr26);
		IDT::set(27, isr27);
		IDT::set(28, isr28);
		IDT::set(29, isr29);
		IDT::set(30, isr30);
		IDT::set(31, isr31);
	}
}