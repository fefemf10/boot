format MS64 COFF
public _idt
section '.text$a' code readable executable align 16
_idt:
	times 4096 db 0
extrn kernel_start
jmp kernel_start