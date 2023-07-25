format MS64 COFF
extrn '?aprunning@APIC@@3EC::<!APIC>' as aprunning
extrn '?bspdone@APIC@@3EA::<!APIC>' as bspdone
section '.apinitc' code readable executable
use64
longMode:
    mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

    mov eax, 1
    cpuid
    shr ebx, 24
    mov rdi, rbx
    ; set up 32k stack, one for each core. It is important that all core must have its own stack
    shl rbx, 15
    mov esp, 0
    sub esp, ebx
    push rdi
    ; spinlock, wait for the BSP to finish
l1: 
    pause
    cmp byte [bspdone], 0
    jz l1
    lock inc byte [aprunning]
    ; jump into C code (should never return)
    jmp $