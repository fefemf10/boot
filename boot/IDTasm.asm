format MS64 COFF

extrn isr1_handler
public loadIDT
public isr1

section '.text$b' code readable executable

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

isr1:
	pushall
	call isr1_handler
	popall
	iretq

loadIDT:
	lidt [idtDescriptor]
	sti
	ret

