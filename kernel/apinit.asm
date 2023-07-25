format binary
CODE_SEG equ 0x0008

use16
org 0x1000
apinit:
	cli
	cld
	jmp 0x0000:apinit1
apinit1:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov al, 0xFF
	out 0xA1, al
	out 0x21, al

	nop
	nop

	lidt [idt1]

	mov eax, 10100000b
	mov cr4, eax

	mov edx, 0x2000;offset to plm
	mov cr3, edx

	mov ecx, 0xC0000080
	rdmsr

	or eax, 0x00000100
	wrmsr

	mov ebx, cr0
	or ebx,0x80000001
	mov cr0, ebx                   

	lgdt [gdtDescriptor1]

	jmp CODE_SEG:longMode
	
gdt1:
	dq 0
	dq 0x00A09A0000000000
	dq 0x00A0920000000000
	dq 0
gdtDescriptor1:
	dw gdtDescriptor1 - gdt1 - 1
	dd gdt1
	dw 0

idt1:
	dw 0
	dd 0

use64
longMode:
jmp [normal_long]
times 256 - 8 - ($ - $$) db 0
normal_long dq 0