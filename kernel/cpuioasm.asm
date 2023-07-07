format MS64 COFF
public outb as '?outb@cpuio@@YAXEG@Z::<!cpuio>'
public outw as '?outw@cpuio@@YAXGG@Z::<!cpuio>'
public outdw as '?outdw@cpuio@@YAXIG@Z::<!cpuio>'
public inb as '?inb@cpuio@@YAEG@Z::<!cpuio>'
public inw as '?inw@cpuio@@YAGG@Z::<!cpuio>'
public indw as '?indw@cpuio@@YAIG@Z::<!cpuio>'
public reboot as '?reboot@cpuio@@YAXXZ::<!cpuio>'
public halt as '?halt@cpuio@@YAXXZ::<!cpuio>'
public loopp as '?loop@cpuio@@YAXXZ::<!cpuio>'
public iowait as '?iowait@cpuio@@YAXXZ::<!cpuio>'
public pausee as '?pause@cpuio@@YAXXZ::<!cpuio>'
public cr22 as '?cr2@cpuio@@YA_KXZ::<!cpuio>'
public cr33 as '?cr3@cpuio@@YA_KXZ::<!cpuio>'
public spp as '?getSP@cpuio@@YA_KXZ::<!cpuio>'
public loadIDTR as '?loadIDTR@cpuio@@YAXPEAX@Z::<!cpuio>'
public loadPLM as '?loadPLM@cpuio@@YAXPEAX@Z::<!cpuio>'
public loadGDT as '?loadGDT@cpuio@@YAXPEAX@Z::<!cpuio>'
public getCPUFeatures as '?getCPUFeatures@cpuio@@YAXAEAUFeatures@1@@Z::<!cpuio>'

section '.text$mn' code readable executable
inb:
	mov dx, cx
	in al, dx
	ret

inw:
	mov dx, cx
	in ax, dx
	ret

indw:
	mov dx, cx
	in eax, dx
	ret

outb:
	mov al, cl
	out dx, al
	ret

outw:
	mov ax, cx
	out dx, ax
	ret

outdw:
	mov eax, ecx
	out dx, eax
	ret

reboot:
	int 0x18
	hlt

halt:
	hlt
	ret

loopp:
	jmp $

iowait:
	mov al, 0
	mov dx, 0x80
	out dx, al 
	ret

pausee:
	pause
	ret

loadIDTR:
	lidt [rcx]
	sti
	ret

loadPLM:
	mov cr3, rcx
	ret

cr22:
	mov rax, cr2
	ret
cr33:
	mov rax, cr3
	ret

spp:
	mov rax, rsp
	ret

loadGDT:
	lgdt [rcx]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	pop rdi
	mov rax, 0x08
	push rax
	push rdi
	retfq

getCPUFeatures:
	mov eax, 1
	mov r8, rcx
	cpuid
	mov dword [r8], ecx 
	mov dword [r8+4], edx
	mov dword [r8+8], eax
	ret