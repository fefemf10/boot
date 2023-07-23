format MS64 COFF
extrn '?gdtDescriptor@GDT@@3UGDTDescriptor@1@A::<!GDT>' as gdtDescriptor
extrn '?idtr@IDT@@3UIDTR@1@A::<!IDT>' as idtr
extrn '?PLM4@memory@@3PEAUPageTable@1@EA::<!memory.Heap>' as PLM4
extrn '?aprunning@APIC@@3EC::<!APIC>' as aprunning
extrn '?bspdone@APIC@@3EA::<!APIC>' as bspdone
section '.apinit' code readable executable
use16
apinit:
    cli
    cld
    xor ax, ax
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    lidt [idtr]
    mov eax, 10100000b
    mov cr4, eax
    mov edx, PLM4
    mov cr3, edx
    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x00000100
    wrmsr
    mov ebx, cr0
    or ebx,0x80000001
    mov cr0, ebx
    lgdt [gdtDescriptor]
    jmp 0x0008:

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
    mov esp, stack_top
    sub esp, ebx
    push rdi
    ; spinlock, wait for the BSP to finish
l1: 
    pause
    cmp [bspdone], 0
    jz l1
    lock inc [aprunning]
    ; jump into C code (should never return)
    jmp ap_startup