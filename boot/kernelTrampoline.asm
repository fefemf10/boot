format MS64 COFF
use64
section '.text' code readable executable

CR4_PGE equ 7
IA32_EFER equ 0xc0000080
IA32_EFER_NX equ 11

public KernelTrampolineStart, KernelTrampolineEnd

align 16
KernelTrampolineStart:
    # Params (Microsoft x64 calling convention)
    #   rcx = BootInfo*
    #   rdx = kernel entry point
    #   r8  = page table

    mov rdi, rcx   # Save BootInfo* and set as param 1 for kernel entry point
    mov rsi, rdx   # Save kernel entry point

    # Enable PGE (Page Global Enable)
    mov rax, cr4
    bts rax, CR4_PGE
    mov cr4, rax

    # Enable NX (No-eXecute)
    mov ecx, IA32_EFER 
    rdmsr
    bts eax, IA32_EFER_NX
    wrmsr

    # Set page table
    mov cr3, r8

    jmp rsi
KernelTrampolineEnd: