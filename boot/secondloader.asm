format MS64 COFF
KERNEL equ 0x10000
extrn kernel_start
section '.text$a' code readable executable align 16
use64
mov rsi, read
mov ah, 0x42
mov dl, 0x80
int 0x13
jmp kernel_start
read:
	db 0x10
	db 0
	dw 0x7F
	dw 0
	dw 10
	dd 2
	dd 0
times 512-($-$$) db 0