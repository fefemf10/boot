format MS64 COFF
use64
section '.text' code readable executable

public setStack as '?setStack@@YAX_K@Z'
public getStack as '?getStack@@YA_KXZ'
setStack:
	mov rsp, rcx
	ret
getStack:
	mov rax, rsp
	ret