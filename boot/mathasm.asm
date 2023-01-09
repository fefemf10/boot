format MS64 COFF
public byteswapu16 as '?byteswap@math@@YAGG@Z::<!math>'
public byteswapu32 as '?byteswap@math@@YAII@Z::<!math>'
public byteswapu64 as '?byteswap@math@@YA_K_K@Z::<!math>'
public absi32 as '?abs@math@@YAHH@Z::<!math>'
public absi64 as '?abs@math@@YA_J_J@Z::<!math>'
public absf32 as '?abs@math@@YAMM@Z::<!math>'
public absf64 as '?abs@math@@YANN@Z::<!math>'

public lerpf32 as '?lerp@math@@YAMMMM@Z::<!math>'

;public cosf32 as '?cos@math@@YAMM@Z::<!math>'
;public sinf32 as '?sin@math@@YAMM@Z::<!math>'
;public tanf32 as '?tan@math@@YAMM@Z::<!math>'
;public acosf32 as '?acos@math@@YAMM@Z::<!math>'
;public asinf32 as '?asin@math@@YAMM@Z::<!math>'
;public atanf32 as '?atan@math@@YAMM@Z::<!math>'
;public atan2f32 as '?atan2@math@@YAMM@Z::<!math>'

public lerpf64 as '?lerp@math@@YANNNN@Z::<!math>'

public expf64 as '?exp@math@@YANN@Z::<!math>'

public powf64 as '?pow@math@@YANNN@Z::<!math>'

public cosf64 as '?cos@math@@YANN@Z::<!math>'
public sinf64 as '?sin@math@@YANN@Z::<!math>'
;public tanf64 as '?tan@math@@YANN@Z::<!math>'
;public acosf64 as '?acos@math@@YANN@Z::<!math>'
;public asinf64 as '?asin@math@@YANN@Z::<!math>'
;public atanf64 as '?atan@math@@YANN@Z::<!math>'
;public atan2f64 as '?atan2@math@@YANN@Z::<!math>'
section '.text$mn' code readable executable
divcosss dq 2.0,24.0,720.0,40320.0,3628800.0,479001600.0,87178291200.0,20922789888000.0,6402373705728000.0
divsinss dq 6.0,120.0,5040.0,362880.0,39916800.0,6227020800.0,1307674368000.0,355687428096000.0,121645100408832000.0

divexpsd dq 2.0,6.0,24.0,120.0,720.0,5040.0,40320.0,362880.0,3628800.0,39916800.0,479001600.0,6227020800.0,87178291200.0,1307674368000.0,20922789888000.0
expsd dq 1.0,2.718281828459045,7.38905609893065,20.085536923187668,54.598150033144236,720.0,5040.0,40320.0,362880.0,3628800.0,39916800.0,479001600.0,6227020800.0,87178291200.0,1307674368000.0,20922789888000.0

divcossd dq 2.0,24.0,720.0,40320.0,3628800.0,479001600.0,87178291200.0,20922789888000.0,6402373705728000.0
divsinsd dq 6.0,120.0,5040.0,362880.0,39916800.0,6227020800.0,1307674368000.0,355687428096000.0,121645100408832000.0
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

lerpf32:
	subss xmm1, xmm0
	mulss xmm2, xmm1
	addss xmm0, xmm2
	ret
;cosf32:
;	sub rsp, 64
;	movaps [rsp+48], xmm6
;	movaps [rsp+32], xmm7
;	movaps [rsp+16], xmm8
;	movaps [rsp], xmm9
;	movaps xmm1, xmm0
;	mulss xmm0, xmm1
;	movaps xmm2, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm3, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm4, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm5, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm6, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm7, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm8, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm9, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	divss xmm0, [divcosss+64]
;	divss xmm9, [divcosss+56]
;	divss xmm8, [divcosss+48]
;	divss xmm7, [divcosss+40]
;	divss xmm6, [divcosss+32]
;	divss xmm5, [divcosss+24]
;	divss xmm4, [divcosss+16]
;	divss xmm3, [divcosss+8]
;	divss xmm2, [divcosss]
;	mov rax,1.0
;	movq xmm1,rax
;	subss xmm1, xmm2
;	addss xmm1, xmm3
;	subss xmm1, xmm4
;	addss xmm1, xmm5
;	subss xmm1, xmm6
;	addss xmm1, xmm7
;	subss xmm1, xmm8
;	addss xmm1, xmm9
;	subss xmm1, xmm0
;	movaps xmm0, xmm1
;	movaps xmm6, [rsp+48]
;	movaps xmm7, [rsp+32]
;	movaps xmm8, [rsp+16]
;	movaps xmm9, [rsp]
;	add rsp, 64
;	ret
;sinf32:
;	sub rsp, 64
;	movaps [rsp+48], xmm6
;	movaps [rsp+32], xmm7
;	movaps [rsp+16], xmm8
;	movaps [rsp], xmm9
;	movaps xmm1, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm2, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm3, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm4, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm5, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm6, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm7, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm8, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	movaps xmm9, xmm0
;	mulss xmm0, xmm1
;	mulss xmm0, xmm1
;	divss xmm0, [divsinss+64]
;	divss xmm7, [divsinss+40]
;	divss xmm6, [divsinss+32]
;	divss xmm5, [divsinss+24]
;	divss xmm4, [divsinss+16]
;	divss xmm3, [divsinss+8]
;	divss xmm2, [divsinss]
;	subss xmm1, xmm2
;	addss xmm1, xmm3
;	subss xmm1, xmm4
;	addss xmm1, xmm5
;	subss xmm1, xmm6
;	addss xmm1, xmm7
;	subss xmm1, xmm8
;	addss xmm1, xmm9
;	subss xmm1, xmm0
;	movaps xmm0, xmm1
;	movaps xmm6, [rsp+48]
;	movaps xmm7, [rsp+32]
;	movaps xmm8, [rsp+16]
;	movaps xmm9, [rsp]
;	add rsp, 64
;	ret
lerpf64:
	subsd xmm1, xmm0
	mulsd xmm2, xmm1
	addsd xmm0, xmm2
	ret
expf64:
	sub rsp, 160
	movapd [rsp+144], xmm6
	movapd [rsp+128], xmm7
	movapd [rsp+112], xmm8
	movapd [rsp+96], xmm9
	movapd [rsp+80], xmm10
	movapd [rsp+64], xmm11
	movapd [rsp+48], xmm12
	movapd [rsp+32], xmm13
	movapd [rsp+16], xmm14
	movapd [rsp], xmm15
	movapd xmm1, xmm0
	mulsd xmm0, xmm1
	movapd xmm2, xmm0
	mulsd xmm0, xmm1
	movapd xmm3, xmm0
	mulsd xmm0, xmm1
	movapd xmm4, xmm0
	mulsd xmm0, xmm1
	movapd xmm5, xmm0
	mulsd xmm0, xmm1
	movapd xmm6, xmm0
	mulsd xmm0, xmm1
	movapd xmm7, xmm0
	mulsd xmm0, xmm1
	movapd xmm8, xmm0
	mulsd xmm0, xmm1
	movapd xmm9, xmm0
	mulsd xmm0, xmm1
	movapd xmm10, xmm0
	mulsd xmm0, xmm1
	movapd xmm11, xmm0
	mulsd xmm0, xmm1
	movapd xmm12, xmm0
	mulsd xmm0, xmm1
	movapd xmm13, xmm0
	mulsd xmm0, xmm1
	movapd xmm14, xmm0
	mulsd xmm0, xmm1
	movapd xmm15, xmm0
	mulsd xmm0, xmm1
	divsd xmm0, [divexpsd+112]
	divsd xmm15, [divexpsd+104]
	divsd xmm14, [divexpsd+96]
	divsd xmm13, [divexpsd+88]
	divsd xmm12, [divexpsd+80]
	divsd xmm11, [divexpsd+72]
	divsd xmm10, [divexpsd+64]
	divsd xmm9, [divexpsd+56]
	divsd xmm8, [divexpsd+48]
	divsd xmm7, [divexpsd+40]
	divsd xmm6, [divexpsd+32]
	divsd xmm5, [divexpsd+24]
	divsd xmm4, [divexpsd+16]
	divsd xmm3, [divexpsd+8]
	divsd xmm2, [divexpsd]
	addsd xmm1, xmm2
	addsd xmm1, xmm3
	addsd xmm1, xmm4
	addsd xmm1, xmm5
	addsd xmm1, xmm6
	addsd xmm1, xmm7
	addsd xmm1, xmm8
	addsd xmm1, xmm9
	addsd xmm1, xmm10
	addsd xmm1, xmm11
	addsd xmm1, xmm12
	addsd xmm1, xmm13
	addsd xmm1, xmm14
	addsd xmm1, xmm15
	addsd xmm1, xmm0
	movapd xmm0, xmm1
	mov rax,1.0
	movq xmm1,rax
	addsd xmm0, xmm1
	movapd xmm6, [rsp+144]
	movapd xmm7, [rsp+128]
	movapd xmm8, [rsp+112]
	movapd xmm9, [rsp+96]
	movapd xmm10, [rsp+80]
	movapd xmm11, [rsp+64]
	movapd xmm12, [rsp+48]
	movapd xmm13, [rsp+32]
	movapd xmm14, [rsp+16]
	movapd xmm15, [rsp]
	add rsp, 160
	ret
powf64:
	; if(xmm0 == 1.0 || xmm1 == 0.0) return;
	movapd xmm3, xmm0
	mov rdx, 1.0
	movq xmm2, rdx
	cmpsd xmm3, xmm2, 0
	movmskpd rax, xmm3
	cmp rax, 1
	je .be
	
	movapd xmm3, xmm1
	mov rdx, 0.0
	movq xmm2, rdx
	cmpsd xmm3, xmm2, 0
	movmskpd rax, xmm3
	cmp rax, 1
	je .be

	;fabs(xmm1) save to xmm3
	xorpd xmm3, xmm3
	subsd xmm3, xmm1
	maxpd xmm3, xmm1

	;convert to integer part exp to rcx
	cvttsd2si rcx, xmm3

	;binarypower
	movapd xmm5, xmm0
	mov rdx, 1.0
	movq xmm2, rdx
	.while:
		test rcx, 1
		je ..skip
		mulsd xmm2, xmm0
		..skip:
		mulsd xmm0, xmm0
		shr rcx, 1
		jnz .while
	movapd xmm0, xmm2

	;save fract part exp
	cvttsd2si rcx, xmm3
	cvtsi2sd xmm4, rcx
	movsd xmm2, xmm3
	subsd xmm2, xmm4
	
	;convert to integer value
	movq rcx, xmm5
	mov rdx, 4606853616395542500
	sub rcx, rdx
	cvtsi2sd xmm5, rcx
	mulsd xmm2, xmm5
	cvtsi2sd xmm5, rdx
	addsd xmm2, xmm5
	cvttsd2si rcx, xmm2
	movq xmm2, rcx
	mulsd xmm0, xmm2

	movapd xmm3, xmm1
	mov rdx, 0.0
	movq xmm2, rdx
	cmpsd xmm3, xmm2, 1
	movmskpd rax, xmm3
	cmp rax, 1
	je .le
	ret
	.be:
		mov rdx, 1.0
		movq xmm0, rdx
		ret
	.le:
		mov rdx, 1.0
		movq xmm2, rdx
		divsd xmm2, xmm0
		movapd xmm0, xmm2
		ret
cosf64:
	sub rsp, 72
	movapd [rsp+48], xmm6
	movapd [rsp+32], xmm7
	movapd [rsp+16], xmm8
	movapd [rsp], xmm9
	movapd xmm1, xmm0
	mulsd xmm0, xmm1
	movapd xmm2, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm3, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm4, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm5, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm6, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm7, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm8, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm9, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	divsd xmm0, [divcossd+64]
	divsd xmm9, [divcossd+56]
	divsd xmm8, [divcossd+48]
	divsd xmm7, [divcossd+40]
	divsd xmm6, [divcossd+32]
	divsd xmm5, [divcossd+24]
	divsd xmm4, [divcossd+16]
	divsd xmm3, [divcossd+8]
	divsd xmm2, [divcossd]
	mov rax,1.0
	movq xmm1,rax
	subsd xmm1, xmm2
	addsd xmm1, xmm3
	subsd xmm1, xmm4
	addsd xmm1, xmm5
	subsd xmm1, xmm6
	addsd xmm1, xmm7
	subsd xmm1, xmm8
	addsd xmm1, xmm9
	subsd xmm1, xmm0
	movapd xmm0, xmm1
	movapd xmm6, [rsp+48]
	movapd xmm7, [rsp+32]
	movapd xmm8, [rsp+16]
	movapd xmm9, [rsp]
	add rsp, 72
	ret
sinf64:
	sub rsp, 72
	movapd [rsp+48], xmm6
	movapd [rsp+32], xmm7
	movapd [rsp+16], xmm8
	movapd [rsp], xmm9
	movapd xmm1, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm2, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm3, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm4, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm5, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm6, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm7, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm8, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	movapd xmm9, xmm0
	mulsd xmm0, xmm1
	mulsd xmm0, xmm1
	divsd xmm0, [divsinsd+64]
	divsd xmm9, [divsinsd+56]
	divsd xmm8, [divsinsd+48]
	divsd xmm7, [divsinsd+40]
	divsd xmm6, [divsinsd+32]
	divsd xmm5, [divsinsd+24]
	divsd xmm4, [divsinsd+16]
	divsd xmm3, [divsinsd+8]
	divsd xmm2, [divsinsd]
	subsd xmm1, xmm2
	addsd xmm1, xmm3
	subsd xmm1, xmm4
	addsd xmm1, xmm5
	subsd xmm1, xmm6
	addsd xmm1, xmm7
	subsd xmm1, xmm8
	addsd xmm1, xmm9
	subsd xmm1, xmm0
	movapd xmm0, xmm1
	movapd xmm6, [rsp+48]
	movapd xmm7, [rsp+32]
	movapd xmm8, [rsp+16]
	movapd xmm9, [rsp]
	add rsp, 72
	ret