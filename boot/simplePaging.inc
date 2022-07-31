pageTableEntry equ 0x1000

setupIndentityPaging:
	; Replace 'pml4_table' with the appropriate physical address and flags, if applicable
	mov edi, pageTableEntry ;plm4t[0]->pdpt
	mov cr3, edi
	xor eax, eax
	mov ecx, 4096
	rep stosd
	mov edi, cr3

	mov [edi], dword 0x2003 ;pdpt[0]->pdt
	add edi, 0x1000
	mov [edi], dword 0x3003 ;pdt[0]->pt
	add edi, 0x1000
	mov [edi], dword 0x4003 ;pt->0x00000000 - 0x00200000
	add edi, 0x1000

	mov ebx, 0x00000003
	mov ecx, 512

	.setEntry:
		mov [edi], dword ebx
		add ebx, 0x1000
		add edi, 8
		loop .setEntry

	; Enable PAE
	mov eax, cr4
	or eax, (1 shl 5)
	mov cr4, eax

	; Set long mode enable
	mov ecx, 0xC0000080 ; Set the C-register to 0xC0000000, which is the EFER MSR
	rdmsr
	or eax, (1 shl 8)
	wrmsr

	; Enable paging and protected mode, if it isn't already active
	mov eax, cr0
	or eax, (1 shl 31)
	mov cr0, eax
	ret