format binary
use16
;add al, al
;add al, bl
;add al, cl
;add al, dl
;add al, ah
;add al, bh
;add al, ch
;add al, dh
;
;add bl, al
;add bl, bl
;add bl, cl
;add bl, dl
;add bl, ah
;add bl, bh
;add bl, ch
;add bl, dh
;
;add cl, al
;add cl, bl
;add cl, cl
;add cl, dl
;add cl, ah
;add cl, bh
;add cl, ch
;add cl, dh
;
;add dl, al
;add dl, bl
;add dl, cl
;add dl, dl
;add dl, ah
;add dl, bh
;add dl, ch
;add dl, dh
;
;add ah, al
;add ah, bl
;add ah, cl
;add ah, dl
;add ah, ah
;add ah, bh
;add ah, ch
;add ah, dh
;
;add bh, al
;add bh, bl
;add bh, cl
;add bh, dl
;add bh, ah
;add bh, bh
;add bh, ch
;add bh, dh
;
;add ch, al
;add ch, bl
;add ch, cl
;add ch, dl
;add ch, ah
;add ch, bh
;add ch, ch
;add ch, dh
;
;add dh, al
;add dh, bl
;add dh, cl
;add dh, dl
;add dh, ah
;add dh, bh
;add dh, ch
;add dh, dh
;
;add [bx+si], ax
;add [bx+di], ax
;add [bp+si], ax
;add [bp+di], ax
;add [si], cx
;add [di], cx
;add [0x5048], cx
;add [bx], cx
;
;add [bx+si+123], ax
;add [bx+di+123], ax
;add [bp+si+123], ax
;add [bp+di+123], ax
;add [si+123], cx
;add [di+123], cx
;add [bp+123], cx
;add [bx+123], cx
;
;add [bx+si+785], ax
;add [bx+di+785], ax
;add [bp+si+785], ax
;add [bp+di+785], ax
;add [si+785], cx
;add [di+785], cx
;add [bp+785], cx
;add [bx+785], cx
;
;add [bx+di+512], cl
;add [bp+si+1024], dl
;add [bp+di+2048], ah
;
;or al, al
;or al, bl
;or al, cl
;or al, dl
;or al, ah
;or al, bh
;or al, ch
;or al, dh
;
;or bl, al
;or bl, bl
;or bl, cl
;or bl, dl
;or bl, ah
;or bl, bh
;or bl, ch
;or bl, dh
;
;or cl, al
;or cl, bl
;or cl, cl
;or cl, dl
;or cl, ah
;or cl, bh
;or cl, ch
;or cl, dh
;
;or dl, al
;or dl, bl
;or dl, cl
;or dl, dl
;or dl, ah
;or dl, bh
;or dl, ch
;or dl, dh
;
;or ah, al
;or ah, bl
;or ah, cl
;or ah, dl
;or ah, ah
;or ah, bh
;or ah, ch
;or ah, dh
;
;or bh, al
;or bh, bl
;or bh, cl
;or bh, dl
;or bh, ah
;or bh, bh
;or bh, ch
;or bh, dh
;
;or ch, al
;or ch, bl
;or ch, cl
;or ch, dl
;or ch, ah
;or ch, bh
;or ch, ch
;or ch, dh
;
;or dh, al
;or dh, bl
;or dh, cl
;or dh, dl
;or dh, ah
;or dh, bh
;or dh, ch
;or dh, dh
;
;or [bx+si], ax
;or [bx+di], ax
;or [bp+si], ax
;or [bp+di], ax
;or [si], cx
;or [di], cx
;or [0x5048], cx
;or [bx], cx
;
;or [bx+si+123], ax
;or [bx+di+123], ax
;or [bp+si+123], ax
;or [bp+di+123], ax
;or [si+123], cx
;or [di+123], cx
;or [bp+123], cx
;or [bx+123], cx
;
;or [bx+si+785], ax
;or [bx+di+785], ax
;or [bp+si+785], ax
;or [bp+di+785], ax
;or [si+785], cx
;or [di+785], cx
;or [bp+785], cx
;or [bx+785], cx
;
;or [bx+di+512], cl
;or [bp+si+1024], dl
;or [bp+di+2048], ah
;
;cmp al, al
;cmp al, bl
;cmp al, cl
;cmp al, dl
;cmp al, ah
;cmp al, bh
;cmp al, ch
;cmp al, dh
;
;cmp bl, al
;cmp bl, bl
;cmp bl, cl
;cmp bl, dl
;cmp bl, ah
;cmp bl, bh
;cmp bl, ch
;cmp bl, dh
;
;cmp cl, al
;cmp cl, bl
;cmp cl, cl
;cmp cl, dl
;cmp cl, ah
;cmp cl, bh
;cmp cl, ch
;cmp cl, dh
;
;cmp dl, al
;cmp dl, bl
;cmp dl, cl
;cmp dl, dl
;cmp dl, ah
;cmp dl, bh
;cmp dl, ch
;cmp dl, dh
;
;cmp ah, al
;cmp ah, bl
;cmp ah, cl
;cmp ah, dl
;cmp ah, ah
;cmp ah, bh
;cmp ah, ch
;cmp ah, dh
;
;cmp bh, al
;cmp bh, bl
;cmp bh, cl
;cmp bh, dl
;cmp bh, ah
;cmp bh, bh
;cmp bh, ch
;cmp bh, dh
;
;cmp ch, al
;cmp ch, bl
;cmp ch, cl
;cmp ch, dl
;cmp ch, ah
;cmp ch, bh
;cmp ch, ch
;cmp ch, dh
;
;cmp dh, al
;cmp dh, bl
;cmp dh, cl
;cmp dh, dl
;cmp dh, ah
;cmp dh, bh
;cmp dh, ch
;cmp dh, dh
;
;cmp [bx+si], ax
;cmp [bx+di], ax
;cmp [bp+si], ax
;cmp [bp+di], ax
;cmp [si], cx
;cmp [di], cx
;cmp [0x5048], cx
;cmp [bx], cx
;
;cmp [bx+si+123], ax
;cmp [bx+di+123], ax
;cmp [bp+si+123], ax
;cmp [bp+di+123], ax
;cmp [si+123], cx
;cmp [di+123], cx
;cmp [bp+123], cx
;cmp [bx+123], cx
;
;cmp [bx+si+785], ax
;cmp [bx+di+785], ax
;cmp [bp+si+785], ax
;cmp [bp+di+785], ax
;cmp [si+785], cx
;cmp [di+785], cx
;cmp [bp+785], cx
;cmp [bx+785], cx
;
;cmp [bx+di+512], cl
;cmp [bp+si+1024], dl
;cmp [bp+di+2048], ah
;
;push bx
;push 123
;push 12345
;
;add al, 5
;imul bx, ax, 5
;imul bx, ax, 269
;
;jo label1
;jno label1
;jb label1
;jnb label1
;jz label1
;jnz label1
;jna label1
;ja label1
;js label1
;jns label1
;jp label1
;jnp label1
;jl label1
;jnl label1
;jng label1
;jg label1
;label1:
;add al, bl
;
;add dl, 8
;or dl, 8
;adc dl, 8
;sbb dl, 8
;and dl, 8
;sub dl, 8
;xor dl, 8
;cmp dl, 8
;
;add bx, 888
;or bx, 888
;adc bx, 888
;sbb bx, 888
;and bx, 888
;sub bx, 888
;xor bx, 888
;cmp bx, 888
;
;add bx, 88
;or bx, 88
;adc bx, 88
;sbb bx, 88
;and bx, 88
;sub bx, 88
;xor bx, 88
;cmp bx, 88
;
;test al, al
;test ax, bx
;test [0x5045], al
;test [0x5045], bx
;
;xchg al, bl
;xchg dx, ax
;xchg dx, bx
;mov ah, bl
;mov [0x55], bl
;mov [0x5512], cx
;mov cl, [0x55]
;mov dx, [0x5550]
;
;cbw
;cwde
;cwd
;cdq
;fwait
;wait
;
;pushf
;popf
;sahf
;lahf
;
;mov al, [0x66]
;mov ax, [0x65]
;mov ax, [0x6665]
;mov [0x66], al
;mov [0x66], ax
;mov [0x5666], ax
;movsb
;movsw
;movsd
;cmpsb
;cmpsw
;cmpsd
;test al, 8
;test ax, 856
;stosb
;stosw
;stosd
;lodsb
;lodsw
;lodsd
;scasb
;scasw
;scasd
;
;mov dl, 24
;mov bx, 568
;mov eax, 568658
;
;rol dl, 3
;ror dh, 4
;rcl bl, 4
;rcr al, 2
;shl dl, 3
;shr bl, 4
;sal al, 2
;sar al, 2
;rol dx, 3
;ror dx, 4
;rcl bx, 4
;rcr ax, 2
;shl dx, 3
;shr bx, 4
;sal ax, 2
;sar ax, 2
;
;rol dl, 1
;ror dh, 1
;rcl bl, 1
;rcr al, 1
;shl dl, 1
;shr bl, 1
;sal al, 1
;sar al, 1
;rol dx, 1
;ror dx, 1
;rcl bx, 1
;rcr ax, 1
;shl dx, 1
;shr bx, 1
;sal ax, 1
;sar ax, 1
;
;rol dl, cl
;ror dh, cl
;rcl bl, cl
;rcr al, cl
;shl dl, cl
;shr bl, cl
;sal al, cl
;sar al, cl
;rol dx, cl
;ror dx, cl
;rcl bx, cl
;rcr ax, cl
;shl dx, cl
;shr bx, cl
;sal ax, cl
;sar ax, cl
;ret 0x56
;ret 0x5655
;
;mov byte [0x55], 8
;mov word [0x5500], 8654
;mov dword [0x556599], 8654
;
;enter 12, 1
;leave
;
;retf 0x5644
;retf
;
;int3
;int 3
;int 32
;iret
;iretd
;loopnz label5
;loopz label5
;loop label5
;label5:
;add al, bl
;add ax, bx
;label3:
;in al, 8
;in ax, 8
;out 8, al
;out 8, ax
;call label3
;jmp label3
;jmp label4
;add ax, bx
;label4:
;add al, bl
;
;and eax, 0xFF55
;push es
;pop es
;
;push ss
;pop ss
;
;push ds
;pop ds
;
int1
icebp
hlt
cmc

test bl, 6
test bh, 7
not bl
neg bl
mul bl
imul bl
div bl
idiv bl


test bx, 0x4566
test bx, 0x4569
not bx
neg bx
mul bx
imul bx
div dx
idiv bx

clc
stc
cld
std
;push fs
;pop fs
;
;push gs
;pop gs
ret