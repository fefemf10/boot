pageTableEntry equ 0x1000

setupIndentityPaging:
	mov edi, pageTableEntry
	mov cr3, edi
	mov dword [edi], 0x2003
	add edi, 0x1000
	mov dword [edi], 0x3003
	add edi, 0x1000
	mov dword [edi], 0x4003
	add edi, 0x1000
	
	mov ebx, 0x00000003
	mov ecx, 512
	
	.setEntry:
		mov dword [edi], ebx
		add ebx, 0x1000
		add edi, 8
		loop .setEntry
	
	mov eax, cr4
	or eax, 32
	mov cr4, eax
	
	mov ecx, 0xC0000080
	rdmsr
	or eax, 256
	wrmsr
	
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	ret