GLOBAL prepareProcess
GLOBAL switchProcess


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
    mov rax, 8 ; align
	push rax
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
    

;switchProcess( uint64_t stackPointer)
switchProcess:

	mov rsp , rdi
	mov al, 20h
	out 20h, al
	
	popState
			

	iretq