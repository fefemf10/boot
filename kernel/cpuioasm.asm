format MS64 COFF
public enableSSE as '?enableSSE@cpuio@@YAXXZ::<!cpuio>'
public enableAVX as '?enableAVX@cpuio@@YAXXZ::<!cpuio>'
public loadIDTR as '?loadIDTR@cpuio@@YAXPEAX@Z::<!cpuio>'
public loadGDT as '?loadGDT@cpuio@@YAXPEAX@Z::<!cpuio>'
public getCPUFeatures as '?getCPUFeatures@cpuio@@YAXAEAUFeatures@1@@Z::<!cpuio>'

section '.text$mn' code readable executable
use16
enableSSE:
	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, (3 shl 9)
	mov cr4, eax
	ret
use64
enableAVX:
	push rax
	push rcx
	push rdx
	
	xor rcx, rcx
	xgetbv
	or eax, 7
	xsetbv
	
	pop rdx
	pop rcx
	pop rax
	ret

loadIDTR:
	lidt [rcx]
	ret

loadGDT:
	lgdt [rcx]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	pop rbp
	mov rax, 0x08
	push rax
	push rbp
	retfq

getCPUFeatures:
	push rax
	push rbx
	push rcx
	push rdx
	mov eax, 1
	mov r8, rcx
	cpuid
	mov dword [r8], ecx 
	mov dword [r8+4], edx
	mov dword [r8+8], eax
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret