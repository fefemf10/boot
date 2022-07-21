format MS64 COFF
org 0x7c00
use16
KERNEL equ 0x8e00
IDT equ 0x7e00

xor ax, ax  ; set up segments
mov ds, ax
mov es, ax
mov ss, ax     ; setup stack
mov sp, 0x7C00
cld
sti
mov bx, word IDT
mov ax, word 0x0213 ; int 13/ah=0x02 BIOS read disk sectors into memory, number of sectors we want to read
mov dh, 0x00 ; head 0, disk 0x?=?
mov cx, word 0x0002; cylinder 0 ;start reading at CL sector (sector 2 in this case, right ater out bootsector)
int 0x13

jmp enterProtectedMode
include 'gdt.inc'

enterProtectedMode:
	; Enable A20
	in al, 0x92
	or al, 0x02
	out 0x92, al

	cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	jmp codeseg:startProtectedMode

use32
include 'cpuid.inc'
include 'simplePaging.inc'

startProtectedMode:
	mov ax, dataseg
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	call detectCPUID
	call detectLongMode
	call setupIndentityPaging
	call editGDT
	jmp codeseg:startLongMode
	
use64
startLongMode:
	call KERNEL
	jmp $
times 510-($-$$) db 0
dw 0xaa55