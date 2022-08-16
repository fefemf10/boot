export module cpuio;
import types;
export namespace cpuio
{
	struct regs
	{
		u64 rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
		//u64 rflags, cr0, cr2, cr3, cr4, cr8;
		u64 interruptCode, errorCode;
	};
	u8 inb(u16 port);
	u16 inw(u16 port);
	u32 indw(u16 port);
	void outb(u8 data, u16 port);
	void outw(u16 data, u16 port);
	void outdw(u32 data, u16 port);
	void reboot();
	void halt();
	void iowait();
}