format MS64 COFF
use64
ATA0_P equ 0x01F0
ATA0_C equ 0x03F6
ATA1_P equ 0x0170
ATA1_C equ 0x0376
SECONDARYBIT equ 0x100
COUNTSECTORS equ 0x80
KERNEL equ 0x100000
section '.flat' code data readable writeable executable
org 0x7e00
mov al, 0x40
mov dx, ATA0_P + 6
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
mov al, 0x80; sectorcount low byte 
out dx, al

mov dx, ATA0_P + 3
mov al, 2; lba1
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

mov cx, 4

.lp1:
	in al, dx
	test al, 0x80 ;BSY bit?
	jne .retry
	test al, 0x08 ;DRQ bit?
	jne .data_ready
.retry:
	dec cx
	jg .lp1

.pior_l:
	in al, dx
	test al, 0x80; BSY
	jne .pior_l
	test al, 0x21; ERR or DF
	jz .data_ready
	jmp $
.data_ready:
mov rax, 0x80; 128 dword
mov rbx, COUNTSECTORS
mul rbx
mov rcx, rax
mov rdi, KERNEL
mov dx, ATA0_P; data i/o
rep insd
jmp KERNEL
times 512-($-$$) db 0
section '.text$a' code readable executable
extrn kernel_start
jmp kernel_start
