format MS64 COFF
public outb
public inb
public retus
section '.text$mn' code readable executable
inb:
	mov dx, cx
	in al, dx
	ret

outb:
	mov al, cl
	out dx, al
	ret

retus:
	mov ax, 'a'
	ret