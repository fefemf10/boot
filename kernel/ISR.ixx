export module ISR;
import types;
import cpuio;
import console;
import Framebuffer;
import intrinsic;
export namespace ISR
{
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
		//framebuffer.clear(console::BLACK);
		//console::color = console::WHITE;
		//console::currentPos = 0;
		console::printf("%s\n", errors[regs.interruptCode]);
		if (regs.interruptCode == 0xC && regs.errorCode != 0)
		{
			if ((regs.errorCode & 0x1)) console::puts("(EXTERNAL)");
			if ((regs.errorCode & 0x6) == 0) console::puts("(GDT)");
			if ((regs.errorCode & 0x6) == 2) console::puts("(IDT)");
			if ((regs.errorCode & 0x6) == 4) console::puts("(LDT)");
			if ((regs.errorCode & 0x6) == 6) console::puts("(IDT)");
			console::printf(" SELECTOR: %llx\n", regs.errorCode & 0xfff8);
		}
		if (regs.interruptCode == 0xE)
		{
			u8 notPresent = !(regs.errorCode & 0x1);
			u8 access = regs.errorCode & 0x2;
			u8 supervisor = regs.errorCode & 0x4;
			u8 reservedBits = regs.errorCode & 0x8;
			u8 instructionFetch = regs.errorCode & 0x10;
			console::printf("CR3: %llx\n", __readcr3());
			console::printf("ADDRESS: %llx\n", __readcr2());
			console::printf("REASON: ");
			if (notPresent) console::puts("(NOT PRESET)");
			if (access) console::puts("(READ-ONLY)");
			if (supervisor) console::puts("(SUPERVISOR)");
			if (reservedBits) console::puts("(RESERVED WRITE)");
			if (instructionFetch) console::puts("(INSTRUCTION FETCH)");
			console::puts("\n");
		}
		console::putregs(regs);
		while (true) _mm_pause();
	}
}