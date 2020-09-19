GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours

section .text

getTime: ; toma como parámetro lo que esté en al
    push rbp
    mov rbp, rsp

	push rcx
	push rbx

		
    out 70h, al ; pido mi item 
    in al, 71h ; guardo mi item en ax
	; 0001 1000
	mov bl, al ; copio
	and bl, 0x0F ; 0000 1000 
	and al, 0xF0 ; 0001 0000
	shr al, 4 ; equiv a hacer >> 4
	mov cl, 10
	mul cl
	add al, bl
	sub al, 3

	pop rbx
	pop rcx

    mov rsp, rbp
    pop rbp 
	ret

getHours:
    mov al, 4
    call getTime
    ret    

getMinutes:
    mov al, 2
    call getTime
    ret   

getSeconds:
	mov al, 0
    call getTime
    ret   