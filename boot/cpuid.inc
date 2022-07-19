detectCPUID:
	pushfd
	pop eax
	
	mov ecx, eax
	
	xor eax, 0x200000
	
	push eax
	popfd
	
	pushfd
	pop eax
	
	push ecx
	popfd
	
	xor eax, ecx
	jz noCPUID
	ret
	
noCPUID:
	hlt
	
detectLongMode:
	mov eax, 0x80000001
	cpuid
	test edx, 0x20000000
	jz noLongMode
	ret
	
noLongMode:
	hlt