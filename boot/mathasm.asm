format MS64 COFF
public absi32 as '?abs@math@@YAHH@Z::<!math>'
public absi64 as '?abs@math@@YA_J_J@Z::<!math>'
public absf32 as '?abs@math@@YANN@Z::<!math>'
public absf64 as '?abs@math@@YAMM@Z::<!math>'
section '.text$mn' code readable executable
absi32:
	mov eax, ecx
	cdq
	xor eax, edx
	sub eax, edx
	ret
absi64:
	mov rax, rcx
	cqo
	xor rax, rdx
	sub rax, rdx
	ret
absf32:
absf64:
	xorps xmm1, xmm1
	subps xmm1, xmm0
	maxps xmm0, xmm1
	ret