format MS64 COFF
extrn '?gdtDescriptor@GDT@@3UGDTDescriptor@1@A::<!GDT>' as gdtDescriptor
extrn '?idtr@IDT@@3UIDTR@1@A::<!IDT>' as idtr
extrn '?PLM4@memory@@3PEAUPageTable@1@EA::<!memory.Heap>' as PLM4
extrn '?aprunning@APIC@@3EC::<!APIC>' as aprunning
extrn '?bspdone@APIC@@3EA::<!APIC>' as bspdone
extrn '?ap_main@APIC@@YAXE@Z::<!APIC>' as ap_main
extrn '?ap_stack@APIC@@3PEAXEA::<!APIC>' as ap_stack
extrn '?ap_stackNumberPagesPerCore@APIC@@3_KA::<!APIC>' as ap_stackNumberPagesPerCore
extrn '?loadGDT@cpuio@@YAXPEAX@Z::<!cpuio>' as loadGDT
public longMode as '?longMode@APIC@@YAXXZ::<!APIC>'
CODE_SEG equ 0x0008
DATA_SEG equ 0x0010
section '.text$mn' code readable executable
use64
times 48 - ($ - $$) db 0
longMode:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	mov rsp, longMode
	mov rcx, gdtDescriptor
	call loadGDT
	
	mov rdx, [PLM4]
	mov cr3, rdx
	
	lidt [idtr]
	
	xor rbx, rbx
	
	mov eax, 1
	cpuid
	mov rax, [ap_stackNumberPagesPerCore]
	shr rbx, 24 ;get apicId
	mov rcx, rbx ;save apicId to ap_main

	mul rbx ;get offset into array stack
	shl rax, 12
	add rax, [ap_stack]
	mov rsp, rax
	sub rsp, 40
spinlock:
	pause
	cmp byte [bspdone], 0
	jz spinlock
	lock inc byte [aprunning]
	jmp ap_main