format MS64 COFF
org 0x7c00
use16
SECONDLOADER equ 0x7e00
KERNEL equ 0x7e00
IDT equ 0x7e00
ATA0_P equ 0x01F0
ATA0_C equ 0x03F6
ATA1_P equ 0x0170
ATA1_C equ 0x0376
LBABIT equ 0xE0
SECONDARYBIT equ 0x100
COUNTSECTORS equ 0x80

xor ax, ax  ; set up segments
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax   ; setup stack
mov sp, 0x7C00
cld
cli

;mov al, dl
;or al, 0xE0
;mov dx, ATA0_P + 6
;out dx, al
;
;mov dx, ATA0_P + 2
;mov al, 0; sectorcount high byte 
;out dx, al
;
;mov dx, ATA0_P + 3
;mov al, 0; lba4
;out dx, al
;
;mov dx, ATA0_P + 4
;mov al, 0; lba5
;out dx, al
;
;mov dx, ATA0_P + 5
;mov al, 0; lba6
;out dx, al
;
;mov dx, ATA0_P + 2
;mov al, COUNTSECTORS; sectorcount low byte 
;out dx, al
;
;mov dx, ATA0_P + 3
;mov al, 1; lba1
;out dx, al
;
;mov dx, ATA0_P + 4
;mov al, 0; lba2
;out dx, al
;
;mov dx, ATA0_P + 5
;mov al, 0; lba3
;out dx, al
;
;mov dx, ATA0_P + 7
;mov al, 0x24; read sectors ext
;out dx, al
;
;mov cx, 4
;
;.lp1:
;	in al, dx
;	test al, 0x80 ;BSY bit?
;	jne .retry
;	test al, 0x08 ;DRQ bit?
;	jne .data_ready
;.retry:
;	dec cx
;	jg .lp1
;
;.pior_l:
;	in al, dx
;	test al, 0x80; BSY
;	jne .pior_l
;	test al, 0x21; ERR or DF
;	jne .fail
;
;.data_ready:
;mov ax, 0x100; 256 word
;mov bx, COUNTSECTORS
;mul bx
;mov cx, ax
;mov di, word IDT
;mov dx, ATA0_P; data i/o
;xor ax, ax
;mov es, ax
;rep insw
;mov dx, ATA0_P + 7
;in al, dx
;in al, dx
;in al, dx
;in al, dx

mov si, readPacketSECONDLOADER
mov ah, 0x42
mov dl, 0x80
int 0x13

jmp enterProtectedMode
use16
.fail:
	mov dx, ATA0_P + 1
	in al, dx
	mov dl, al
	mov si, strr
	call print
	jmp $

print:
   lodsb
   or al, al  ;zero=end of str
   jz done    ;get out
   mov ah, 0x0E
   mov bh, 0
   int 0x10
   jmp print
done:
   ret
strr db "stop", 0

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