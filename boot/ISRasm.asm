format MS64 COFF
section '.text$mn' code readable executable
extrn isrHandler
macro pushall
{
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rsp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
}
macro popall
{
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rsp
	pop rbp
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
}
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
isr0:
	pushq 0
	pushq 0
	jmp isrCommon
isr1:
	pushq 0
	pushq 1
	jmp isrCommon
isr2:
	pushq 0
	pushq 2
	jmp isrCommon
isr3:
	pushq 0
	pushq 3
	jmp isrCommon
isr4:
	pushq 0
	pushq 4
	jmp isrCommon
isr5:
	pushq 0
	pushq 5
	jmp isrCommon
isr6:
	pushq 0
	pushq 6
	jmp isrCommon
isr7:
	pushq 0
	pushq 7
	jmp isrCommon
isr8:
	pushq 8
	jmp isrCommon
isr9:
	pushq 0
	pushq 9
	jmp isrCommon
isr10:
	pushq 10
	jmp isrCommon
isr11:
	pushq 11
	jmp isrCommon
isr12:
	pushq 12
	jmp isrCommon
isr13:
	pushq 13
	jmp isrCommon
isr14:
	pushq 14
	jmp isrCommon
isr15:
	pushq 0
	pushq 15
	jmp isrCommon
isr16:
	pushq 0
	pushq 16
	jmp isrCommon
isr17:
	pushq 0
	pushq 17
	jmp isrCommon
isr18:
	pushq 0
	pushq 18
	jmp isrCommon
isr19:
	pushq 0
	pushq 19
	jmp isrCommon
isr20:
	pushq 0
	pushq 20
	jmp isrCommon
isr21:
	pushq 0
	pushq 21
	jmp isrCommon
isr22:
	pushq 0
	pushq 22
	jmp isrCommon
isr23:
	pushq 0
	pushq 23
	jmp isrCommon
isr24:
	pushq 0
	pushq 24
	jmp isrCommon
isr25:
	pushq 0
	pushq 25
	jmp isrCommon
isr26:
	pushq 0
	pushq 26
	jmp isrCommon
isr27:
	pushq 0
	pushq 27
	jmp isrCommon
isr28:
	pushq 0
	pushq 28
	jmp isrCommon
isr29:
	pushq 0
	pushq 29
	jmp isrCommon
isr30:
	pushq 0
	pushq 30
	jmp isrCommon
isr31:
	pushq 0
	pushq 31
	jmp isrCommon
isrCommon:
	pushall
	lea rcx, [rsp]
	call isrHandler
	popall
	add rsp, 16
	iretq