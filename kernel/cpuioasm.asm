format MS64 COFF
public enableSSE as '?enableSSE@cpuio@@YAXXZ::<!cpuio>'
public enableAVX as '?enableAVX@cpuio@@YAXXZ::<!cpuio>'
public loadIDTR as '?loadIDTR@cpuio@@YAXPEAX@Z::<!cpuio>'
public loadGDT as '?loadGDT@cpuio@@YAXPEAX@Z::<!cpuio>'
public isEnabledInterrupt as '?isEnabledInterrupt@cpuio@@YA_NXZ::<!cpuio>'

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

isEnabledInterrupt:
	pushfq
	pop rax
	and rax, 1000000000b
	shr rax, 9
	ret