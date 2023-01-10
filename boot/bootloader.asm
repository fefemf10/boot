format MS64 COFF
READBLOCKCOUNT equ 0x2
MAXCOUNTSECTORS equ 0x7f
SECONDLOADER equ 0x8000
KERNEL equ 0x100000
section '.text$a' code readable executable
use16
org 0x7c00
xor ax, ax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0x7C00
cld
cli

push dx

;; GET MEMORY MAP
memory_entries equ 0x27C00; 2:7a00
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
	mov ax, 0xB800
	mov es, ax
	mov di, 2
	mov [es:di], byte 'B'
	jmp $
.done:
	mov ax, ((memory_entries and 0xFF0000) shr 4)
	mov es, ax
	mov di, (memory_entries and 0xFFFF)
	mov [es:di], bp
	clc

;read 0x7F SECTORS
pop dx
mov bx, dx
xor ax, ax
mov ds, ax
mov ah, 0x42
mov si, readPacket0
int 0x13
mov cx, READBLOCKCOUNT
.diskread:
mov dx, bx
mov ah, 0x42
mov si, readPacket1
int 0x13
loop .diskread

call setVESA

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
	mov rdi, KERNEL + 0x660
	mov rcx, (MAXCOUNTSECTORS * READBLOCKCOUNT * 0x200 / 8)
	rep movsq
	call KERNEL + 0x660

readPacket0:
	db 0x10
	db 0
	dw 1
	dw 0x7E00
	dw 0
	dd 1
	dd 0
readPacket1:
	db 0x10
	db 0
	dw MAXCOUNTSECTORS
	dw SECONDLOADER
	dw 0
	dd 2
	dd 0
times 510-($-$$) db 0
dw 0xaa55

use16
vesa equ 0x27A00
vesaModes equ 0x27C00
vesaMaxCountModes equ 20
vesaModesSizeof equ 256
setVESA:
	.getVESAInfo:
		mov edi, vesa
		mov ax, 0x4F00
		int 0x10
	.findVESAModes:
		mov edi, vesa + 14
		mov ax, [es:di]
		mov si, ax
		add di, 2
		mov ax, [es:di]
		mov fs, ax
		mov edi, vesaModes + 6
		mov ax, (((vesaModes + 6) and 0xFF0000) shr 4)
		mov ds, ax
		xor dx, dx
		xor bx, bx
		mov ds, dx
		..loop:
			mov cx, word [fs:si]
			cmp cx, 0xFFFF
			je ..getVESAModesInfo
			add si, 2
			inc dx
			mov ax, 0x4F01
			int 0x10
			mov ax, word [es:di]
			bt ax, 7
			jnc ...skip3
			add di, 25
			mov al, byte [es:di]
			cmp al, [.bpp]
			jne ...skip1
			sub di, 25
			add di, 18
			mov ax, word [es:di]
			cmp ax, [.width]
			jb ...skip2
			sub di, 18
			mov [.width], ax
			mov [.currentMode], dx
			dec [.currentMode]
			mov bx, cx
			...skip1:
			sub di, 25
			jmp ...skip3
			...skip2:
			sub di, 18
			...skip3:
			add edi, vesaModesSizeof
			jmp ..loop
			
		..getVESAModesInfo:
			mov edi, vesaModes
			mov [es:di], dx
	.setVESAMode:
		mov ax, 0x4F02
		;or bx, 1 shl 14
		int 0x10
		mov ax, [.currentMode]
		mov [es:di+4], ax
	ret
.width dw 0
.height dw 0
.bpp db 32
.currentMode dw 0
.vesaModesAddrSegment dw 0
.vesaModesAddrOffset dw 0
times 1024-($-$$) db 0