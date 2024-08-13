format MS64 COFF
section '.text$mn' code readable executable align 16
public memcpy as '?copy@memory@@YAPEAXPEAXPEBX_K@Z::<!memory.utils>'
public memcpy
public memmove as '?move@memory@@YAPEAXPEAXPEBX_K@Z::<!memory.utils>'
public memmove
;extrn __favor:byte
;extrn __isa_available:dword
__FAVOR_ENFSTRG equ 2
__ISA_AVAILABLE_AVX equ 8
memcpy_repmovs:
	push rdi
    push rsi
    mov rdi, rcx
    mov rsi, rdx
    mov rcx, r8
    rep movsb
    pop rsi
    pop rdi
    ret
memmove:
memcpy:
	mov rax, rcx
	cmp r8, 15
	ja MoveAbove15
	
	align 16

	mov r9, MoveSmall
	jmp qword [r9 + r8*8]
	MoveSmall0:
		ret
	
	align 16
	
	MoveSmall15:
		mov r8, [rdx]
        mov ecx, [rdx + 8]
        movzx r9d, word [rdx + 12]
        movzx r10d, byte [rdx + 14]
        mov [rax], r8
        mov [rax + 8], ecx
        mov [rax + 12], r9w
        mov [rax + 14], r10b
        ret
	MoveSmall11:
		mov r8, [rdx]
        movzx ecx, word [rdx + 8]
        movzx r9d, byte [rdx + 10]
        mov [rax], r8
        mov [rax + 8], cx
        mov [rax + 10], r9b
        ret
	MoveSmall2:
		movzx ecx, word [rdx]
        mov [rax], cx
        ret
		
	align 16	
		
	MoveSmall7:
		mov ecx, [rdx]
        movzx r8d, word [rdx + 4]
        movzx r9d, byte [rdx + 6]
        mov [rax], ecx
        mov [rax + 4], r8w
        mov [rax + 6], r9b
        ret
	MoveSmall14:
        mov r8, [rdx]
        mov ecx, [rdx + 8]
        movzx r9d, word [rdx + 12]
        mov [rax], r8
        mov [rax + 8], ecx
        mov [rax + 12], r9w
        ret
	MoveSmall3:
        movzx ecx, word [rdx]
        movzx r8d, byte [rdx + 2]
        mov [rax], cx
        mov [rax + 2], r8b
        ret

        align 16
		
	MoveSmall13:
        mov r8, [rdx]
        mov ecx, [rdx + 8]
        movzx r9d, byte [rdx + 12]
        mov [rax], r8
        mov [rax + 8], ecx
        mov [rax + 12], r9b
        ret
	MoveSmall10:
        mov r8, [rdx]
        movzx ecx, word [rdx + 8]
        mov [rax], r8
        mov [rax + 8], cx
        ret
	MoveSmall9:
        mov r8, [rdx]
        movzx ecx, byte [rdx + 8]
        mov [rax], r8
        mov [rax + 8], cl
        ret
	MoveSmall12:
        mov r8, [rdx]
        mov ecx, [rdx + 8]
        mov [rax], r8
        mov [rax + 8], ecx
        ret

        align 16
		
	MoveSmall6:
        mov ecx, [rdx]
        movzx r8d, word [rdx + 4]
        mov [rax], ecx
        mov [rax + 4], r8w
        ret
	MoveSmall5:
        mov ecx, [rdx]
        movzx r8d, byte [rdx + 4]
        mov [rax], ecx
        mov [rax + 4], r8b
        ret
	MoveSmall8:
        mov rcx, [rdx]
        mov [rax], rcx
        ret
	MoveSmall1:
        movzx ecx, byte [rdx]
        mov [rax], cl
        ret
	MoveSmall4:
        mov ecx, [rdx]
        mov [rax], ecx
        ret
	
	align 16

	MoveAbove15:
		cmp r8, 32
		ja MoveAbove32

	; set blocks between 16 and 32 bytes in length

		movdqu xmm1, [rdx]
        movdqu xmm2, [rdx + r8 - 16]
        movdqu [rcx], xmm1
        movdqu [rcx + r8 - 16], xmm2
        ret
		
	MoveAbove32:
        lea r9, [rdx + r8]
        cmp rcx, rdx
        cmovbe r9, rcx
        cmp      rcx, r9
        jb       CopyDown

        jmp      NoAVX
	; SSE based implementation

	align 16
	
	NoAVX:
		SSE_LEN_BIT equ 4
		SSE_STEP_LEN equ 1 SHL SSE_LEN_BIT
		SSE_LOOP_LEN equ SSE_STEP_LEN*8
		KB equ 1024
		FAST_STRING_SSE_THRESHOLD equ 2 * KB
		cmp r8, FAST_STRING_SSE_THRESHOLD ; jbe 128 byte set
		jbe MoveWithXMM
		
        ;test [__favor], __FAVOR_ENFSTRG
		jmp memcpy_repmovs 
		
	MoveWithXMM:
        movdqu   xmm0, [rdx - SSE_STEP_LEN * 0]
        movdqu   xmm5, [rdx + r8 - SSE_STEP_LEN * 1]
        cmp      r8, SSE_LOOP_LEN
        jbe      MovUpTo128WithXMM

        mov      r9, rcx
        and      r9, SSE_STEP_LEN-1
        sub      r9, SSE_STEP_LEN
        sub      rcx, r9
        sub      rdx, r9
        add      r8, r9
        cmp      r8, SSE_LOOP_LEN
        jbe      MovUpTo128WithXMM

	; set > 128 bytes with an SSE based loop

	align 16
	XmmLoop:
		movdqu xmm1, [rdx + SSE_STEP_LEN * 0]
		movdqu xmm2, [rdx + SSE_STEP_LEN * 1]
		movdqu xmm3, [rdx + SSE_STEP_LEN * 2]
		movdqu xmm4, [rdx + SSE_STEP_LEN * 3]
		movdqa [rcx + SSE_STEP_LEN * 0], xmm1
		movdqa [rcx + SSE_STEP_LEN * 1], xmm2
		movdqa [rcx + SSE_STEP_LEN * 2], xmm3
		movdqa [rcx + SSE_STEP_LEN * 3], xmm4
		movdqu xmm1, [rdx + SSE_STEP_LEN * 4]
		movdqu xmm2, [rdx + SSE_STEP_LEN * 5]
		movdqu xmm3, [rdx + SSE_STEP_LEN * 6]
		movdqu xmm4, [rdx + SSE_STEP_LEN * 7]
		movdqa [rcx + SSE_STEP_LEN * 4], xmm1
		movdqa [rcx + SSE_STEP_LEN * 5], xmm2
		movdqa [rcx + SSE_STEP_LEN * 6], xmm3
		movdqa [rcx + SSE_STEP_LEN * 7], xmm4
		add rcx, SSE_LOOP_LEN
		add rdx, SSE_LOOP_LEN
		sub r8, SSE_LOOP_LEN
		cmp r8, SSE_LOOP_LEN
		jae XmmLoop
		
	; set up to 128 bytes using SSE stores
	MovUpTo128WithXMM:
		lea r9, [r8 + SSE_STEP_LEN-1]
		and r9, -SSE_STEP_LEN
		mov r11, r9
		shr r11, SSE_LEN_BIT
		mov r10, MoveSmallXmm
		mov r11, [r10 + r11*8]
		jmp r11
	Mov8XmmBlocks:
		movdqu xmm1, [rdx + r9 - SSE_STEP_LEN * 8]
		movdqu [rcx + r9 - SSE_STEP_LEN * 8], xmm1
	Mov7XmmBlocks:
		movdqu xmm1, [rdx + r9 - SSE_STEP_LEN * 7]
		movdqu [rcx + r9 - SSE_STEP_LEN * 7], xmm1
	Mov6XmmBlocks:
		movdqu xmm1, [rdx + r9 - SSE_STEP_LEN * 6]
		movdqu [rcx + r9 - SSE_STEP_LEN * 6], xmm1
	Mov5XmmBlocks:
		movdqu xmm1, [rdx + r9 - SSE_STEP_LEN * 5]
		movdqu [rcx + r9 - SSE_STEP_LEN * 5], xmm1
	Mov4XmmBlocks:
		movdqu xmm1, [rdx + r9 - SSE_STEP_LEN * 4]
		movdqu [rcx + r9 - SSE_STEP_LEN * 4], xmm1
	Mov3XmmBlocks:
		movdqu xmm1, [rdx + r9 - SSE_STEP_LEN * 3]
		movdqu [rcx + r9 - SSE_STEP_LEN * 3], xmm1
	Mov2XmmBlocks:
		movdqu xmm1, [rdx + r9 - SSE_STEP_LEN * 2]
		movdqu [rcx + r9 - SSE_STEP_LEN * 2], xmm1
	Mov1XmmBlocks:
		movdqu [rcx + r8 - SSE_STEP_LEN * 1], xmm5
	Mov0XmmBlocks:
		movdqu [rax], xmm0
		ret
		
	align 16
	CopyDown:
        mov r11, rcx
        mov r10, rdx
        sub rdx, rcx
        add rcx, r8
        movups xmm0, [rcx + rdx - 16]
        sub rcx, 16
        sub r8, 16 
		
		test cl, 15
        jz XmmMovLargeTest
		
	XmmMovAlign:
        mov rax, rcx
        and rcx, -16
        movups xmm1, xmm0
        movups xmm0, [rcx + rdx]
        movups [rax], xmm1
        mov r8, rcx
        sub r8, r11
		

; See if we can move any 128-byte blocks.  

XmmMovLargeTest:                           
        mov      r9, r8 
        shr      r9, 7 
        jz       XmmMovSmallTest 
        movaps   [rcx], xmm0
        jmp      XmmMovLargeInner

	; Move 128-byte blocks                     

        align    16                        
		
	XmmMovLargeOuter:
        movaps   [rcx + 128 - 112], xmm0
        movaps   [rcx + 128 - 128], xmm1
	XmmMovLargeInner:
        movups   xmm0, [rcx + rdx - 16]
        movups   xmm1, [rcx + rdx - 32]
        sub      rcx, 128                  
        movaps   [rcx + 128 - 16], xmm0       
        movaps   [rcx + 128 - 32], xmm1       
        movups   xmm0, [rcx + rdx + 128 - 48] 
        movups   xmm1, [rcx + rdx + 128 - 64] 
        dec      r9                        
        movaps   [rcx + 128 - 48], xmm0       
        movaps   [rcx + 128 - 64], xmm1       
        movups   xmm0, [rcx + rdx + 128 - 80] 
        movups   xmm1, [rcx + rdx + 128 - 96] 
        movaps   [rcx + 128 - 80], xmm0       
        movaps   [rcx + 128 - 96], xmm1       
        movups   xmm0, [rcx + rdx + 128 - 112]
        movups   xmm1, [rcx + rdx + 128 - 128]
        jnz      XmmMovLargeOuter

        movaps   [rcx + 128-112], xmm0
        and      r8, 127
        movaps   xmm0, xmm1

; See if we have any 16-byte blocks left to move

XmmMovSmallTest:
        mov r9, r8
        shr r9, 4
        jz XmmMovTrailing

        align 16

XmmMovSmallLoop:
        movups [rcx], xmm0
        sub rcx, 16
        movups xmm0, [rcx + rdx]
        dec r9
        jnz XmmMovSmallLoop

XmmMovTrailing:
        and r8, 15
        jz XmmMovReturn
		
        movups xmm1, [r10]
        movups [r11], xmm1

	XmmMovReturn:
        movups [rcx], xmm0
        mov rax, r11
        ret

MoveSmall:
	dq MoveSmall0
	dq MoveSmall1
	dq MoveSmall2
	dq MoveSmall3
	dq MoveSmall4
	dq MoveSmall5
	dq MoveSmall6
	dq MoveSmall7
	dq MoveSmall8
	dq MoveSmall9
	dq MoveSmall10
	dq MoveSmall11
	dq MoveSmall12
	dq MoveSmall13
	dq MoveSmall14
	dq MoveSmall15
MoveSmallXmm:
	dq Mov0XmmBlocks
	dq Mov1XmmBlocks
	dq Mov2XmmBlocks
	dq Mov3XmmBlocks
	dq Mov4XmmBlocks
	dq Mov5XmmBlocks
	dq Mov6XmmBlocks
	dq Mov7XmmBlocks
	dq Mov8XmmBlocks