format MS64 COFF

extrn isr32_handler
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
public isr32
public isr

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
isr:
dq isr0 - $
dq isr1 - $
dq isr2 - $
dq isr3 - $
dq isr4 - $
dq isr5 - $
dq isr6 - $
dq isr7 - $
dq isr8 - $
dq isr9 - $
dq isr10 - $
dq isr11 - $
dq isr12 - $
dq isr13 - $
dq isr14 - $
dq isr15 - $
dq isr16 - $
dq isr17 - $
dq isr18 - $
dq isr19 - $
dq isr20 - $
dq isr21 - $
dq isr22 - $
dq isr23 - $
dq isr24 - $
dq isr25 - $
dq isr26 - $
dq isr27 - $
dq isr28 - $
dq isr29 - $
dq isr30 - $
dq isr31 - $

isr32:
	pushall
	cld
	call isr32_handler
	popall
	iretq

isr0:
	pushall
	cld
	call isr0_handler
	popall
	iret
isr1:
	pushall
	cld
	call isr1_handler
	popall
	iret
isr2:
	pushall
	cld
	call isr2_handler
	popall
	iret
isr3:
	pushall
	cld
	call isr3_handler
	popall
	iret
isr4:
	pushall
	cld
	call isr4_handler
	popall
	iret
isr5:
	pushall
	cld
	call isr5_handler
	popall
	iret
isr6:
	pushall
	cld
	call isr6_handler
	popall
	iret
isr7:
	pushall
	cld
	call isr7_handler
	popall
	iret
isr8:
	pushall
	cld
	call isr8_handler
	popall
	iret
isr9:
	pushall
	cld
	call isr9_handler
	popall
	iret
isr10:
	pushall
	cld
	call isr10_handler
	popall
	iret
isr11:
	pushall
	cld
	call isr11_handler
	popall
	iret
isr12:
	pushall
	cld
	call isr12_handler
	popall
	iret
isr13:
	pushall
	cld
	call isr13_handler
	popall
	iret
isr14:
	pushall
	cld
	call isr14_handler
	popall
	iret
isr15:
	pushall
	cld
	call isr15_handler
	popall
	iret
isr16:
	pushall
	cld
	call isr16_handler
	popall
	iret
isr17:
	pushall
	cld
	call isr17_handler
	popall
	iret
isr18:
	pushall
	cld
	call isr18_handler
	popall
	iret
isr19:
	pushall
	cld
	call isr19_handler
	popall
	iret
isr20:
	pushall
	cld
	call isr20_handler
	popall
	iret
isr21:
	pushall
	cld
	call isr21_handler
	popall
	iret
isr22:
	pushall
	cld
	call isr22_handler
	popall
	iret
isr23:
	pushall
	cld
	call isr23_handler
	popall
	iret
isr24:
	pushall
	cld
	call isr24_handler
	popall
	iret
isr25:
	pushall
	cld
	call isr25_handler
	popall
	iret
isr26:
	pushall
	cld
	call isr26_handler
	popall
	iret
isr27:
	pushall
	cld
	call isr27_handler
	popall
	iret
isr28:
	pushall
	cld
	call isr28_handler
	popall
	iret
isr29:
	pushall
	cld
	call isr29_handler
	popall
	iret
isr30:
	pushall
	cld
	call isr30_handler
	popall
	iret
isr31:
	pushall
	cld
	call isr31_handler
	popall
	iret
loadIDT:
	lidt [idtDescriptor]
	sti
	ret

