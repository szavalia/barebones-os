GLOBAL atomix_add 
GLOBAL next_round
GLOBAL stop_interrupts
GLOBAL xchange


section .text

;extern int atomix_add(int value , void * place )
;rdi 
atomix_add:
    mov rax , rdi
    lock xadd [rsi] , eax
    ret
;extern void next_round()

xchange:
    mov rax , rdi ;// value
    lock xchg [rsi] , eax
    ret
next_round:
    
    mov al, 20h
    out 20h, al
    sti
    int 20h


;extern void stop_interrupts()
stop_interrupts:
    cli