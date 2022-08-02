format MS64 COFF
use64
ATA0_P equ 0x01F0
ATA0_C equ 0x03F6
ATA1_P equ 0x0170
ATA1_C equ 0x0376
COUNTSECTORS equ 0x7F
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
mov al, COUNTSECTORS; sectorcount low byte 
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
mov rdi, KERNEL
mov r8, COUNTSECTORS
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
	jne .fail
.data_ready:
mov rcx, 0x80; 128 dword
mov dx, ATA0_P; data i/o
rep insd
mov dx, ATA0_P + 7
in al, dx
in al, dx
in al, dx
in al, dx
dec r8
test r8, r8
jnz .pior_l
cli
cld
jmp KERNEL
.fail:
mov dx, ATA0_P + 1
in al, dx
in al, dx
in al, dx
in al, dx
xor rcx, rcx
mov cl, al
printhexb:
    mov rax, rcx
    mov rcx, 8
    mov r8, 0xB801E
.loopp:
    mov dl, al
    and dl, 0x0F
    shr rax, 4
    call print_nibble
    mov dl, al
    and dl, 0x0F
    shr rax, 4
    call print_nibble
    loop .loopp
    jmp $
print_nibble:
    cmp dl, 0x09
    jg .letter
    mov r9b, 0x30
    add r9b, dl
    mov [r8], r9b
    sub r8, 2
    ret
.letter:
    mov r9b, 0x37
    add r9b, dl
    mov [r8], r9b
    sub r8, 2
    ret
times 512-($-$$) db 0
section '.text$a' code readable executable
extrn kernel_start
call kernel_start
jmp $