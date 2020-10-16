GLOBAL prepareProcess
GLOBAL switchProcess
GLOBAL prepareProcessForked

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

section .text

;llena el stack de forma correcta
; extern void prepareProcess( int PID , uint64_t stackPointer , int argc , char * argv[] , void * main );
; rdi -> PID , rsi -> stackPointer , rbx -> argc , rcx ->argv , r8 -> main
prepareProcess:
    mov rbp , rsi
    mov rsp , rsi
    
    ;push exit_fnc
    ;mov rax, 8 ; align
	;push rax
    mov rax, 0x0 ; SS
    push rax      
    push rsi ; rsp
    mov rax,0x202  ;RFLAGS
    push rax
    mov rax, 0x8 ; CS
    push rax 
    push r8 ; main del proceso
    
    ;push de los registros 
    push rdi ;rax
    push rdi ;rbx
    push rdi ;rcx
    push rdi ;rdx
    push rbp ;rbp
    push rbx ;rdi --> argc
    push rcx ;rsi --> argv
    push rdi
    push rdi
    push rdi
    push rdi
    push rdi
    push rdi
    push rdi
    push rdi
	mov al, 20h
	out 20h, al
    popState
    iretq
 
;   rdi ---> stackPointer del nuevo proceso
; 	rsi ---> basePointer del viejo
;	rbx ---> basePointer del nuevo
prepareProcessForked:
	pushState	
	mov r8 , rdi
	mov rcx , 18 ; cant de regs pusheados
.loop:
	mov rax , [rdi+8] ;
	mov [rdi] , rax
	add rdi , 8
	dec rcx 
	cmp rcx , 0
	jne .loop

	mov QWORD[rdi] , 0 ; fork child
	add rdi , 8 ; rip
	add rdi , 8 ; CS
	add rdi , 8 ; RFLAGS
	add rdi , 8 ; rsp --> STACK DEL PADRE
	mov rcx , [rdi] ;
	sub rsi , rcx ; creo el OFFSET
	sub rbx , rsi; base pointer hijo - offset, paro el rsp del hijo donde tiene que ir
	mov [rdi] , rbx;

	popState
	ret 
	 


;switchProcess( uint64_t stackPointer)
switchProcess:

	mov rsp , rdi
	sti
	mov al, 20h
	out 20h, al
	
	popState
	
	iretq