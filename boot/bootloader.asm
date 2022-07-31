format MS64 COFF
org 0x7c00
use16
SECONDLOADER equ 0x7e00

xor ax, ax  ; set up segments
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax   ; setup stack
mov sp, 0x7C00
cld
cli
mov si, readPacketSECONDLOADER
mov ah, 0x42
int 0x13

jmp enterProtectedMode

include 'gdt.inc'

enterProtectedMode:
	; Enable A20
	in al, 0x92
	or al, 0x02
	out 0x92, al
	
	cli

	; Disable NMI
	in al, 0x70
	or al, 0x80
	out 0x70, al
	in al, 0x71

	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	; Enable SSE
	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, (3 shl 9)
	mov cr4, eax

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
	jmp SECONDLOADER
readPacketSECONDLOADER:
	db 0x10
	db 0
	dw 1
	dw SECONDLOADER
	dw 0
	dd 1
	dd 0
times 510-($-$$) db 0
dw 0xaa55