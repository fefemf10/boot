format MS64 COFF
public outb
public outw
public inb
public inw
public reboot
public iowait

section '.text$mn' code readable executable
inb:
	mov dx, cx
	in al, dx
	ret

inw:
	mov dx, cx
	in ax, dx
	ret

outb:
	mov al, cl
	out dx, al
	ret

outw:
	mov ax, cx
	out dx, ax
	ret

reboot:
	int 0x18
	hlt

iowait:
	mov al, 0
	mov dx, 0x80
	out dx, al 
	ret