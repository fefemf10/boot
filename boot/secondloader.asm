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
call clear

mov r11, 0xB801E

mov rcx, rdx
call printhexb

mov dx, ATA0_P + 7
in al, dx
mov rcx, rax
call printhexb

mov al, 0xA0
mov dx, ATA0_P + 6
out dx, al

mov al, 0
mov dx, ATA0_P + 2
out dx, al

mov dx, ATA0_P + 3
out dx, al

mov dx, ATA0_P + 4
out dx, al

mov dx, ATA0_P + 5
out dx, al

mov al, 0xEC
mov dx, ATA0_P + 7
out dx, al

in al, dx

mov rcx, rax
call printhexb

mov dx, ATA0_P + 4
in al, dx

mov rcx, rax
call printhexb

mov dx, ATA0_P + 5
in al, dx

mov rcx, rax
call printhexb

jmp $
;
;
;

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
mov r11, 0xB801E
.lp1:
	in al, dx
    mov rcx, rax
    call printhexb
    add r11, 0x20
	test al, 0x80 ;BSY bit?
	jnz .lp1
	test al, 0x08 ;DRQ bit?
	jz .lp1

.pior_l:
	in al, dx
	test al, 0x80; BSY
	jnz .pior_l
	test al, 0x21; ERR or DF
	jnz .fail
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
call printhexb
jmp $
printhexb:
    ;r8 arg
    ;r9 temp
    ;r10 temp
    ;r11 videobuffer
    push r8
    mov r8, rcx
    mov rcx, 8
.loopp:
    call print_nibble
    call print_nibble
    loop .loopp
    pop r8
    add r11, 0x42
    ret
print_nibble:
    mov r9b, r8b
    shr r8, 4
    and r9b, 0x0F
    cmp r9b, 0x09
    jg .letter
    mov r10b, 0x30
    add r10b, r9b
    mov [r11], r10b
    sub r11, 2
    ret
.letter:
    mov r10b, 0x37
    add r10b, r9b
    mov [r11], r10b
    sub r11, 2
    ret
clear:
    mov rdi, 0xb8000
    mov rcx, 2000
    mov ax, 0x0F00
    rep stosw
    ret
times 512-($-$$) db 0
section '.text$a' code readable executable
extrn kernel_start
call kernel_start
jmp $