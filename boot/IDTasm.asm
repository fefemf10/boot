format MS64 COFF
section '.text$b' code readable executable
extrn isr33_handler
extrn isr0_handler
extrn isr1_handler
extrn isr2_handler
extrn isr3_handler
extrn isr4_handler
extrn isr5_handler
extrn isr6_handler
extrn isr7_handler
extrn isr8_handler
extrn isr9_handler
extrn isr10_handler
extrn isr11_handler
extrn isr12_handler
extrn isr13_handler
extrn isr14_handler
extrn isr15_handler
extrn isr16_handler
extrn isr17_handler
extrn isr18_handler
extrn isr19_handler
extrn isr20_handler
extrn isr21_handler
extrn isr22_handler
extrn isr23_handler
extrn isr24_handler
extrn isr25_handler
extrn isr26_handler
extrn isr27_handler
extrn isr28_handler
extrn isr29_handler
extrn isr30_handler
extrn isr31_handler

public loadIDT
public isr33
public isr0
public isr1
public isr2
public isr3
public isr4
public isr5
public isr6
public isr7
public isr8
public isr9
public isr10
public isr11
public isr12
public isr13
public isr14
public isr15
public isr16
public isr17
public isr18
public isr19
public isr20
public isr21
public isr22
public isr23
public isr24
public isr25
public isr26
public isr27
public isr28
public isr29
public isr30
public isr31


idtDescriptor:
	dw 4095
	dq 0x7e00

macro pushall
{
	push rax
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
}

macro popall
{
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rax
}

isr33:
	pushall
	call isr33_handler
	popall
	iretq

isr0:
	pushall
	cld
	call isr0_handler
	popall
	iretq
isr1:
	pushall
	cld
	call isr1_handler
	popall
	iretq
isr2:
	pushall
	cld
	call isr2_handler
	popall
	iretq
isr3:
	pushall
	cld
	call isr3_handler
	popall
	iretq
isr4:
	pushall
	cld
	call isr4_handler
	popall
	iretq
isr5:
	pushall
	cld
	call isr5_handler
	popall
	iretq
isr6:
	pushall
	cld
	call isr6_handler
	popall
	iretq
isr7:
	pushall
	cld
	call isr7_handler
	popall
	iretq
isr8:
	pushall
	cld
	call isr8_handler
	popall
	iretq
isr9:
	pushall
	cld
	call isr9_handler
	popall
	iretq
isr10:
	pushall
	cld
	call isr10_handler
	popall
	iretq
isr11:
	pushall
	cld
	call isr11_handler
	popall
	iretq
isr12:
	pushall
	cld
	call isr12_handler
	popall
	iretq
isr13:
	pushall
	cld
	call isr13_handler
	popall
	iretq
isr14:
	pushall
	cld
	call isr14_handler
	popall
	iretq
isr15:
	pushall
	cld
	call isr15_handler
	popall
	iretq
isr16:
	pushall
	cld
	call isr16_handler
	popall
	iretq
isr17:
	pushall
	cld
	call isr17_handler
	popall
	iretq
isr18:
	pushall
	cld
	call isr18_handler
	popall
	iretq
isr19:
	pushall
	cld
	call isr19_handler
	popall
	iretq
isr20:
	pushall
	cld
	call isr20_handler
	popall
	iretq
isr21:
	pushall
	cld
	call isr21_handler
	popall
	iretq
isr22:
	pushall
	cld
	call isr22_handler
	popall
	iretq
isr23:
	pushall
	cld
	call isr23_handler
	popall
	iretq
isr24:
	pushall
	cld
	call isr24_handler
	popall
	iretq
isr25:
	pushall
	cld
	call isr25_handler
	popall
	iretq
isr26:
	pushall
	cld
	call isr26_handler
	popall
	iretq
isr27:
	pushall
	cld
	call isr27_handler
	popall
	iretq
isr28:
	pushall
	cld
	call isr28_handler
	popall
	iretq
isr29:
	pushall
	cld
	call isr29_handler
	popall
	iretq
isr30:
	pushall
	cld
	call isr30_handler
	popall
	iretq
isr31:
	pushall
	cld
	call isr31_handler
	popall
	iretq
loadIDT:
	lidt [idtDescriptor]
	sti
	ret