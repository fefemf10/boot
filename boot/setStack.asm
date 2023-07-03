format MS64 COFF
use64
section '.text' code readable executable

extrn printf_unsigned
extrn '?mainCRTStartup@@3P6AXAEAUBootInfo@@@ZEA' as main:qword
public setStack as '?setStack@@YAXPEAXPEAUBootInfo@@@Z'
public getStack as '?getStack@@YA_KXZ'
setStack:
	mov rsp, rcx
	mov rcx, rdx
	;movapd xmm0, [rdx]
	;movapd [rsp], xmm0
	;
	;movapd xmm0, [rdx+16]
	;movapd [rsp+16], xmm0
	;
	;movapd xmm0, [rdx+32]
	;movapd [rsp+32], xmm0
	;
	;movapd xmm0, [rdx+48]
	;movapd [rsp+48], xmm0
	;
	;movapd xmm0, [rdx+64]
	;movapd [rsp+64], xmm0
	;
	;movapd xmm0, [rdx+80]
	;movapd [rsp+80], xmm0
	;
	;movapd xmm0, [rdx+96]
	;movapd [rsp+96], xmm0
	jmp qword [main]
getStack:
	mov rax, rsp
	ret