format MS64 COFF
READCOUNT equ 0x2
COUNTSECTORS equ 0x7F
SECONDLOADER equ 0x7e00
KERNEL equ 0x100000
section '.text$a' code readable executable
use16
org 0x7c00
xor ax, ax  ; set up segments
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax   ; setup stack
mov sp, 0x7C00
cld
cli
push dx
;; GET MEMORY MAP
memory_entries equ 0x27A00; 2:7a00
getMemoryMap:
	mov ax, ((memory_entries and 0xFF0000) shr 4)
	mov es, ax
	mov di, ((memory_entries + 4) and 0xFFFF)
	xor ebx, ebx
	xor bp, bp
	mov edx, 'PAMS'
	mov eax, 0xE820
	mov [es:di+20], dword 1
	mov ecx, 24
	
	int 0x15
	jc .error
	
	cmp eax, 'PAMS'
	jne .error
	test ebx, ebx
	jz .error
	jmp .start
	
.next_entry:
	mov edx, 'PAMS'
	mov ecx, 24
	mov eax, 0xE820
	int 0x15
.start:
	jcxz .skip_entry
	mov ecx, [es:di+8]
	or ecx, [es:di+12]
	jz .skip_entry

.good_entry:
	inc bp
	add di, 24
	
.skip_entry:
	test ebx, ebx
	jz .done
	jmp .next_entry
.error:
	stc
.done:
	mov ax, ((memory_entries and 0xFF0000) shr 4)
	mov es, ax
	mov di, (memory_entries and 0xFFFF)
	mov [es:di], bp
	clc

;read 0x7F + 0x7F SECTORS
pop dx
xor ax, ax
mov es, ax
mov ah, 0x42
mov si, readPacket
int 0x13
add [readPacket + 4], word 0xFE00 ; COUNTSECTORS*512
add [readPacket + 6], word 0x1000
add [readPacket + 8], dword COUNTSECTORS
int 0x13

jmp enterProtectedMode

gdt_nulldesc:
	dd 0
	dd 0
gdt_codedesc:
	dw 0xFFFF ; limit
	dw 0x0000 ; base low
	db 0x00 ; base medium
	db 10011010b ; flags
	db 11001111b ; flags upper limit
	db 0x00 ; base hiht
gdt_datadesc:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 10010010b
	db 11001111b
	db 0x00
gdt_end:
gdt_descriptor:
	gdt_size:
		dw gdt_end - gdt_nulldesc - 1
		dq gdt_nulldesc
codeseg equ gdt_codedesc - gdt_nulldesc
dataseg equ gdt_datadesc - gdt_nulldesc

enterProtectedMode:
	; Enable A20
	in al, 0x92
	or al, 0x02
	out 0x92, al
	
	cli

	; Disable NMI
	in al, 0x70
	or al, 0x80
	out 0x70, al
	in al, 0x71

	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	
	; Enable SSE
	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, (3 shl 9)
	mov cr4, eax

	jmp codeseg:startProtectedMode

use32
pageTableEntry equ 0x1000
setupIndentityPaging:
	; Replace 'pml4_table' with the appropriate physical address and flags, if applicable
	mov edi, pageTableEntry ;plm4t[0]->pdpt
	mov cr3, edi
	xor eax, eax
	mov ecx, 4096
	rep stosd
	mov edi, cr3

	mov [edi], dword 0x2003 ;pdp[0]->pdt
	add edi, 0x1000
	mov [edi], dword 0x3003 ;pd[0]->ptt
	add edi, 0x1000
	mov [edi], dword 0x4003 ;pt[0]->0x00000000 - 0x00200000
	;mov [edi+8], dword 0x5003 ;pt[1]->0x00200000 - 0x00400000
	add edi, 0x1000

	mov ebx, 0x00000003
	mov ecx, 512

	.setEntry:
		mov [edi], dword ebx
		add ebx, 0x1000
		add edi, 8
		loop .setEntry
	
	;mov ecx, 512
	;
	;.setEntry1:
	;	mov [edi], dword ebx
	;	add ebx, 0x1000
	;	add edi, 8
	;	loop .setEntry1

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

editGDT:
	mov [gdt_codedesc + 6], byte 10101111b
	mov [gdt_datadesc + 6], byte 10101111b
	ret

startProtectedMode:
	mov ax, dataseg
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	call setupIndentityPaging
	call editGDT
	jmp codeseg:startLongMode
use64
startLongMode:
	mov rsi, SECONDLOADER
	mov rdi, KERNEL + 0x460
	mov rcx, (COUNTSECTORS * READCOUNT * 0x200 / 8)
	rep movsq
	jmp KERNEL + 0x460
readPacket:
	db 0x10
	db 0
	dw COUNTSECTORS
	dw SECONDLOADER
	dw 0
	dd 1
	dd 0
times 510-($-$$) db 0
dw 0xaa55