format MS64 COFF
public outb
public outw
public outdw
public inb
public inw
public indw
public reboot
public iowait
public loadIDTR

section '.text$mn' code readable executable
inb:
	mov dx, cx
	in al, dx
	ret

inw:
	mov dx, cx
	in ax, dx
	ret

indw:
	mov dx, cx
	in eax, dx
	ret

outb:
	mov al, cl
	out dx, al
	ret

outw:
	mov ax, cx
	out dx, ax
	ret

outdw:
	mov eax, ecx
	out dx, eax
	ret

reboot:
	int 0x18
	hlt

iowait:
	mov al, 0
	mov dx, 0x80
	out dx, al 
	ret

loadIDTR:
	
	lidt [rcx]
	sti
	ret