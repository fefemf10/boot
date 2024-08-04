format MS64 COFF
section '.text$mn' code readable executable
extrn irqHandler
public irqHandlers as '?irqHandlers@IDT@@3PAP6AXXZA'

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
irqHandlers:
	dq irq0
	dq irq1
	dq irq2
	dq irq3
	dq irq4
	dq irq5
	dq irq6
	dq irq7
	dq irq8
	dq irq9
	dq irq10
	dq irq11
	dq irq12
	dq irq13
	dq irq14
	dq irq15
irq0:
	pushq 0
	pushq 32
	jmp irqCommon
irq1:
	pushq 0
	pushq 33
	jmp irqCommon
irq2:
	pushq 0
	pushq 34
	jmp irqCommon
irq3:
	pushq 0
	pushq 35
	jmp irqCommon
irq4:
	pushq 0
	pushq 36
	jmp irqCommon
irq5:
	pushq 0
	pushq 37
	jmp irqCommon
irq6:
	pushq 0
	pushq 38
	jmp irqCommon
irq7:
	pushq 0
	pushq 39
	jmp irqCommon
irq8:
	pushq 0
	pushq 40
	jmp irqCommon
irq9:
	pushq 0
	pushq 41
	jmp irqCommon
irq10:
	pushq 0
	pushq 42
	jmp irqCommon
irq11:
	pushq 0
	pushq 43
	jmp irqCommon
irq12:
	pushq 0
	pushq 44
	jmp irqCommon
irq13:
	pushq 0
	pushq 45
	jmp irqCommon
irq14:
	pushq 0
	pushq 46
	jmp irqCommon
irq15:
	pushq 0
	pushq 47
	jmp irqCommon
irqCommon:
	pushall
	lea rcx, [rsp]
	call irqHandler
	popall
	add rsp, 16
	iretq