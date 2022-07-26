format MS64 COFF
org 0x7c00
use16
KERNEL equ 0x8e00
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

mov al, dl
mov dx, ATA0_P + 6
or al, LBABIT
out dx, al

mov dx, ATA0_P + 2
mov al, 0; sectorcount high byte 
out dx, al

mov dx, ATA0_P + 3
mov al, 0; lba4
out dx, al

mov dx, ATA0_P + 4
mov al, 0; lba5
out dx, al

mov dx, ATA0_P + 5
mov al, 0; lba6
out dx, al

mov dx, ATA0_P + 2
mov al, COUNTSECTORS; sectorcount low byte 
out dx, al

mov dx, ATA0_P + 3
mov al, 1; lba1
out dx, al

mov dx, ATA0_P + 4
mov al, 0; lba2
out dx, al

mov dx, ATA0_P + 5
mov al, 0; lba3
out dx, al

mov dx, ATA0_P + 7
mov al, 0x24; read sectors ext
out dx, al

.loop:
	in al, dx
	test al, 0x80 ;BSY bit?
	jnz .loop
	test al, 0x08 ;DRQ bit?
	jz .loop

.loop1:
	test al, 0x21;ERR and DF
	jnz .fail
	
	
.data_ready:
mov ax, 0x100; 256 word
mov bx, COUNTSECTORS
mul bx
mov cx, ax
mov di, word IDT
mov dx, ATA0_P; data i/o
xor ax, ax
mov es, ax
rep insw


;mov bx, word IDT
;mov ah, 0x02
;mov al, COUNTSECTORS; int 13/ah=0x02 BIOS read disk sectors into memory, number of sectors we want to read
;mov dh, 0x00 ; head 0, disk 0x?=?
;mov cx, word 0x0002; cylinder 0 ;start reading at CL sector (sector 2 in this case, right ater out bootsector)
;int 0x13



jmp enterProtectedMode
use16
.fail:
	mov dx, ATA0_P + 1
	in al, dx
	mov dl, al
	jmp $
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