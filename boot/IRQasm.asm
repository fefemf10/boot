format MS64 COFF
section '.text$mn' code readable executable
extrn irqHandler
macro pushall
{
	;push rax
	;mov rax, cr8
	;push rax
	;mov rax, cr4
	;push rax
	;mov rax, cr3
	;push rax
	;mov rax, cr2
	;push rax
	;mov rax, cr0
	;push rax
	;pushfq
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
	;popfq
	;pop cr0
	;pop cr2
	;pop cr3
	;pop cr4
	;pop cr8
}
public irq0
public irq1
public irq2
public irq3
public irq4
public irq5
public irq6
public irq7
public irq8
public irq9
public irq10
public irq11
public irq12
public irq13
public irq14
public irq15
irq0:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 32
	jmp irqCommon
irq1:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 33
	jmp irqCommon
irq2:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 34
	jmp irqCommon
irq3:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 35
	jmp irqCommon
irq4:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 36
	jmp irqCommon
irq5:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 37
	jmp irqCommon
irq6:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 38
	jmp irqCommon
irq7:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 39
	jmp irqCommon
irq8:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 40
	jmp irqCommon
irq9:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 41
	jmp irqCommon
irq10:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 42
	jmp irqCommon
irq11:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 43
	jmp irqCommon
irq12:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 44
	jmp irqCommon
irq13:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 45
	jmp irqCommon
irq14:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 46
	jmp irqCommon
irq15:
	push rbp
	mov rbp, rsp
	pushq 0
	pushq 47
	jmp irqCommon
irqCommon:
	pushall
	lea rcx, [rsp]
	call irqHandler
	popall
	add rsp, 16
	mov rsp, rbp
	pop rbp
	iretq