format MS64 COFF
public byteswapu16 as '?byteswap@math@@YAGG@Z::<!math>'
public byteswapu32 as '?byteswap@math@@YAII@Z::<!math>'
public byteswapu64 as '?byteswap@math@@YA_K_K@Z::<!math>'
public absi32 as '?abs@math@@YAHH@Z::<!math>'
public absi64 as '?abs@math@@YA_J_J@Z::<!math>'
public absf32 as '?abs@math@@YANN@Z::<!math>'
;public cosf32 as '?cos@math@@YANN@Z::<!math>'
;public sinf32 as '?sin@math@@YANN@Z::<!math>'
;public tanf32 as '?tan@math@@YANN@Z::<!math>'
;public acosf32 as '?acos@math@@YANN@Z::<!math>'
;public asinf32 as '?asin@math@@YANN@Z::<!math>'
;public atanf32 as '?atan@math@@YANN@Z::<!math>'
;public atan2f32 as '?atan2@math@@YANN@Z::<!math>'
public absf64 as '?abs@math@@YAMM@Z::<!math>'
;public cosf64 as '?cos@math@@YAMM@Z::<!math>'
;public sinf64 as '?sin@math@@YAMM@Z::<!math>'
;public tanf64 as '?tan@math@@YAMM@Z::<!math>'
;public acosf64 as '?acos@math@@YAMM@Z::<!math>'
;public asinf64 as '?asin@math@@YAMM@Z::<!math>'
;public atanf64 as '?atan@math@@YAMM@Z::<!math>'
;public atan2f64 as '?atan2@math@@YAMM@Z::<!math>'
section '.text$mn' code readable executable
byteswapu16:
	mov ax, cx
	rol ax, 8
	ret
byteswapu32:
	mov eax, ecx
	bswap eax
	ret
byteswapu64:
	mov rax, rcx
	bswap rax
	ret
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