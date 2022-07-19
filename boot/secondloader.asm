format MS64 COFF
section '.text$a' code readable executable
extrn kernel_start
jmp kernel_start