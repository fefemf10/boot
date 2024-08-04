format MS64 COFF
use64
section '.text' code readable executable
extrn '?mainCRTStartup@@3P6AXAEAUBootInfo@@@ZEA' as main:qword
public setStack as '?setStack@@YAXPEAXPEAUBootInfo@@@Z'
public getStack as '?getStack@@YA_KXZ'
setStack:
	mov rsp, rcx
	mov rcx, rdx
	sub rsp, 40
	jmp qword [main]
getStack:
	mov rax, rsp
	ret