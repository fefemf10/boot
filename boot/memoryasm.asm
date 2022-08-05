format MS64 COFF
section '.text$mn' code readable executable align 16
extrn  __ImageBase
public memset
memset_repstos:
	push rdi
	mov eax, edx
	mov rdi, rcx
	mov rcx, r8
	rep stosb
	mov rax, r9
	pop rdi
	ret
memset:
	mov rax, rcx
	mov r9, rcx
	lea r10, [__ImageBase]
	movzx edx, dl
	mov r11, 0101010101010101h
	imul r11, rdx
	movq xmm0, r11
	cmp r8, 15
	ja SetAbove15
	
	align 16

	add rcx, r8
	mov r9d, [SetSmall - $$ + r10 + r8*4]
	add r9, r10
	jmp r9
	SetSmall15:
		mov [rcx-15], r11
	SetSmall7:
		mov [rcx-7], r11d
	SetSmall3:
		mov [rcx-3], r11w
	SetSmall1:
		mov [rcx-1], r11b
	SetSmall0:
		ret
	SetSmall14:
		mov [rcx-14], r11
	SetSmall6:
		mov [rcx-6], r11d
	SetSmall2:
		mov [rcx-2], r11w
	MoveSmall0:
		ret

	align 16

	SetSmall13:
		mov [rcx-13], r11
	SetSmall5:
		mov [rcx-5], r11d
		mov [rcx-1], r11b
		ret

	align 16

	SetSmall12:
		mov [rcx-12], r11
	SetSmall4:
		mov [rcx-4], r11d
		ret
	SetSmall11:
		mov [rcx-11], r11
		mov [rcx-3], r11w
		mov [rcx-1], r11b
		ret
	SetSmall9:
		mov [rcx-9], r11
		mov [rcx-1], r11b
		ret

	align 16
	SetSmall10:
		mov [rcx-10], r11
		mov [rcx-2], r11w
		ret
	SetSmall8:
		mov [rcx-8], r11
		ret
	
	align 16

	SetAbove15:
		punpcklqdq xmm0, xmm0
		cmp r8, 32
		ja NoAVX

	; set blocks between 16 and 32 bytes in length

		movdqu [rcx], xmm0
		movdqu [rcx + r8 - 16], xmm0
		ret

	; SSE based implementation

	align 16
	NoAVX:
		SSE_LEN_BIT equ 4
		SSE_STEP_LEN equ 1 SHL SSE_LEN_BIT
		SSE_LOOP_LEN equ SSE_STEP_LEN*8
		cmp r8, SSE_LOOP_LEN ; jbe 128 byte set
		jbe SetWithXMM

	; set blocks between 16 and 32 bytes in length
		jnz memset_repstos
	
	SetWithXMM:
		mov r9, rcx
		and r9, SSE_STEP_LEN-1
		sub r9, SSE_STEP_LEN
		sub rcx, r9
		
		sub rdx, r9
		add r8, r9
		cmp r8, SSE_LOOP_LEN
		jbe SetUpTo128WithXMM

	; set > 128 bytes with an SSE based loop

	align 16
	XmmLoop:
		movdqa [rcx + SSE_STEP_LEN * 0], xmm0
		movdqa [rcx + SSE_STEP_LEN * 1], xmm0
		movdqa [rcx + SSE_STEP_LEN * 2], xmm0
		movdqa [rcx + SSE_STEP_LEN * 3], xmm0
		movdqa [rcx + SSE_STEP_LEN * 4], xmm0
		movdqa [rcx + SSE_STEP_LEN * 5], xmm0
		movdqa [rcx + SSE_STEP_LEN * 6], xmm0
		movdqa [rcx + SSE_STEP_LEN * 7], xmm0
		add rcx, SSE_LOOP_LEN
		sub r8, SSE_LOOP_LEN
		cmp r8, SSE_LOOP_LEN
		jae XmmLoop

	; set up to 128 bytes using SSE stores
	SetUpTo128WithXMM:
		lea r9, [r8 + SSE_STEP_LEN-1]
		and r9, -SSE_STEP_LEN
		mov r11, r9
		shr r11, SSE_LEN_BIT
		mov r11d, [SetSmallXmm - $$ + r10 + r11*4]
		add r11, r10
		jmp r11
	Set8XmmBlocks:
		movdqu [rcx + r9 - SSE_STEP_LEN * 8], xmm0
	Set7XmmBlocks:
		movdqu [rcx + r9 - SSE_STEP_LEN * 7], xmm0
	Set6XmmBlocks:
		movdqu [rcx + r9 - SSE_STEP_LEN * 6], xmm0
	Set5XmmBlocks:
		movdqu [rcx + r9 - SSE_STEP_LEN * 5], xmm0
	Set4XmmBlocks:
		movdqu [rcx + r9 - SSE_STEP_LEN * 4], xmm0
	Set3XmmBlocks:
		movdqu [rcx + r9 - SSE_STEP_LEN * 3], xmm0
	Set2XmmBlocks:
		movdqu [rcx + r9 - SSE_STEP_LEN * 2], xmm0
	Set1XmmBlocks:
		movdqu [rcx + r8 - SSE_STEP_LEN * 1], xmm0
	Set0XmmBlocks:
		movdqu [rax], xmm0
		ret

SetSmall:
	dd SetSmall0
	dd SetSmall1
	dd SetSmall2
	dd SetSmall3
	dd SetSmall4
	dd SetSmall5
	dd SetSmall6
	dd SetSmall7
	dd SetSmall8
	dd SetSmall9
	dd SetSmall10
	dd SetSmall11
	dd SetSmall12
	dd SetSmall13
	dd SetSmall14
	dd SetSmall15
SetSmallXmm:
	dd Set8XmmBlocks
	dd Set7XmmBlocks
	dd Set6XmmBlocks
	dd Set5XmmBlocks
	dd Set4XmmBlocks
	dd Set3XmmBlocks
	dd Set2XmmBlocks
	dd Set1XmmBlocks
	dd Set0XmmBlocks