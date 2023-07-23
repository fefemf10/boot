format MS64 COFF
section '.text$mn' code readable executable align 16
public memcmp as '?cmp@memory@@YAHPEBX0_K@Z::<!memory.utils>'
public memcmp as '?memcmp@@YAHPEBX0_K@Z::<!sl.memory>'
public memcmp
memcmp:
	sub rdx, rcx
	cmp r8, 8
	jb short mcmp30
	test cl, 7
	jz short mcmp20

	align 16

mcmp10:
	mov al, [rcx]
	cmp al, [rcx + rdx]
	jne short mcmp_not_equal
	inc rcx
	dec r8
	test cl, 7
	jne short mcmp10

mcmp20:
	mov r9, r8
	shr r9, 3
	jnz short mcmp50

mcmp30:
	test r8, r8
	jz short mcmp_equal
mcmp40:
	mov al, [rcx]
	cmp al, [rcx + rdx]
	jne short mcmp_not_equal
	inc rcx
	dec r8
	jnz short mcmp40
mcmp_equal:
	xor rax, rax
	ret
mcmp_not_equal:
	sbb eax, eax
	sbb eax, -1
	ret

	db      090h

mcmp50:
	shr r9, 2
	jz short mcmp70
mcmp60:
	mov rax, [rcx]
	cmp rax, [rcx + rdx]
	jne mcmp_adjust0
	mov rax, [rcx + 8]
	cmp rax, [rcx + rdx + 8]
	jne mcmp_adjust8
	mov rax, [rcx + 16]
	cmp rax, [rcx + rdx + 16]
	jne mcmp_adjust16
	mov rax, [rcx + 24]
	cmp rax, [rcx + rdx + 24]
	jne mcmp_adjust24
	add rcx, 32
	dec r9
	jnz short mcmp60
	and r8, 31
mcmp70:
	mov r9, r8
	shr r9, 3
	jz  short mcmp30
mcmp80:
	mov rax, [rcx]
	cmp rax, [rcx + rdx]
	jne short mcmp_adjust0
	add rcx, 8
	dec r9
	jnz short mcmp80
	and r8, 7
	jmp mcmp30
mcmp_adjust24:
	add rcx, 8

mcmp_adjust16:
	add rcx, 8

mcmp_adjust8:
	add rcx, 8

mcmp_adjust0:
	mov rcx, [rdx + rcx]
	bswap rax
	bswap rcx
	cmp rax, rcx
	sbb eax, eax
	sbb eax, -1
	ret