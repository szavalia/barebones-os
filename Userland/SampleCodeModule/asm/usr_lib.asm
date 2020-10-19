GLOBAL put
GLOBAL scanChar
GLOBAL getTime
GLOBAL getReg
GLOBAL getCPUInfo
GLOBAL getMem
GLOBAL getTemp
GLOBAL getContext
GLOBAL callMalloc
GLOBAL changeContext
GLOBAL callFree
GLOBAL codeError
GLOBAL mem
GLOBAL callPs
GLOBAL callNice
GLOBAL callKill
GLOBAL callLaunch
GLOBAL callLoop
GLOBAL getPID
GLOBAL callExit
GLOBAL renounceCPU
GLOBAL blockProcess
GLOBAL callSemInit
GLOBAL callSemPost
GLOBAL callSemWait
GLOBAL callSemState
GLOBAL pipeWrite
GLOBAL pipeRead
GLOBAL pipeOpen
GLOBAL pipeClose
GLOBAL callPipe
GLOBAL change_input
GLOBAL change_output


;Acá vamos a poner los llamados al SO para interactuar con el hardware
section .text


;invalid opcode
codeError: 
        UD2
       

;void scanChar(char * destination); 
scanChar:
    push rbp
    mov rbp, rsp
    
    push r12
    push r13 

    mov r12, 0
    mov r13, rdi ;puntero a mi char (uno solito)
    int 80h

    pop r13
    pop r12
    
    mov rsp, rbp
    pop rbp
    ret  

;void put(char * source, int size)
put:
    push rbp
    mov rbp, rsp

    push r12
    push r13
    push r15

    mov r12, 1
    mov r13, rdi
    mov r15, rsi
    int 80h

    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

getReg:
    push rbp
    mov rbp, rsp

    push r12
    push r13

    mov r12, 2 ; elegi que este sea el codigo por que era el siguiente en el orden
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

    

;void getTime(int * destination) 
getTime:
    push rbp
    mov rbp, rsp

    push r12
    push r13

    mov r12, 3 ; elegi que este sea el codigo por que era el siguiente en el orden
    mov r13, rdi ; le paso un int * para guardar la hora
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret


getMem:
    push rbp
    mov rbp, rsp

    push r12
    push r13
    push r15

    mov r12, 4 
    mov r13, rdi
    mov r15, rsi
    int 80h

    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret 

getCPUInfo:
    push rbp
    mov rbp, rsp

    push r12
    push r13
    push r15

    mov r12, 5 
    mov r13, rdi
    mov r15, rsi
    int 80h

    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret 
          
getTemp:
    push rbp
    mov rbp, rsp

    push r12
    push r13

    mov r12, 6
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret 

getContext:
    push rbp
    mov rbp, rsp

    push r12
    push r13

    mov r12, 7
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret 


changeContext:
    push rbp
    mov rbp, rsp

    push r12
    push r13

    mov r12, 8
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret 


callMalloc:
    push rbp
    mov rbp, rsp 

    push r12
    push r13 ;primer param, size
    push r15 ;segundo param, el void *

    mov r12, 9
    mov r13, rdi
    mov r15, rsi
    int 80h

    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret 
    
callFree:
    push rbp
    mov rbp, rsp 

    push r12
    push r13 

    mov r12, 10
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret 

mem:
    push rbp
    mov rbp, rsp 

    push r12

    mov r12, 11
    int 80h

    pop r12

    mov rsp, rbp
    pop rbp
    ret

callPs:
    push rbp
    mov rbp, rsp 

    push r12

    mov r12, 12
    int 80h

    pop r12

    mov rsp, rbp
    pop rbp
    ret


callKill:
    push rbp
    mov rbp, rsp 

    push r12
    push r13 ;primer param, pid

    mov r12, 13
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret  

callLaunch:
    push rbp
    mov rbp, rsp 

    push r12
    push r13 
    push rbx 
    push rcx

    mov r12, 14
    mov r13, rdi
    mov r15, rsi
    mov rbx, rdx
    mov r10, rcx
    int 80h

    pop rcx
    pop rbx 
    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

getPID:
    push rbp
    mov rbp, rsp 

    push r12
    push r13

    mov r12, 15
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

callLoop:
    push rbp
    mov rbp, rsp 

    push r12

    mov r12, 16
    int 80h

    pop r12

    mov rsp, rbp
    pop rbp
    ret

callExit:
    push rbp
    mov rbp, rsp 

    push r12

    mov r12, 17
    int 80h

    pop r12

    mov rsp, rbp
    pop rbp
    ret

callSemInit:
    push rbp
    mov rbp, rsp 

    push r12
    push r13
    push r15

    mov r12, 18
    mov r13, rdi ; sem pointer pointer
    mov r15, rsi ; value
    int 80h

    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

callSemWait:
    push rbp
    mov rbp, rsp 

    push r12
    push r13

    mov r12, 19
    mov r13 , rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret




 callSemPost:
    push rbp
    mov rbp, rsp 

    push r12
    push r13


    mov r12, 20
    mov r13 , rdi

    int 80h

    pop r13
    pop r12


    mov rsp, rbp
    pop rbp
    ret
renounceCPU:
    push rbp
    mov rbp, rsp 

    push r12

    mov r12, 21
    int 80h

    pop r12

    mov rsp, rbp
    pop rbp
    ret

blockProcess:
    push rbp
    mov rbp, rsp 

    push r12
    push r13

    mov r12, 22
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret


callNice:
    push rbp
    mov rbp, rsp 

    push r12
    push r13 ;primer param, pid
    push r15 ;segundo param, new_prio

    mov r12, 23
    mov r13, rdi
    mov r15, rsi
    int 80h

    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

pipeWrite:
    push rbp
    mov rbp, rsp 

    push r12
    push r13
    push r15
    push rbx

    mov r12, 24
    mov r13, rdi
    mov r15, rsi
    mov rbx, rdx
    int 80h

    pop rbx 
    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

pipeRead:
    push rbp
    mov rbp, rsp 

    push r12
    push r13 ;primer param, size
    push r15 ;segundo param, el void *
    push rbx

    mov r12, 25
    mov r13, rdi
    mov r15, rsi
    mov rbx, rdx
    int 80h

    pop rbx 
    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

pipeOpen:
    push rbp
    mov rbp, rsp 

    push r12
    push r13

    mov r12, 26
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

pipeClose:
    push rbp
    mov rbp, rsp 

    push r12
    push r13

    mov r12, 27
    mov r13, rdi
    int 80h

    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

callPipe:
    push rbp
    mov rbp, rsp 

    push r12

    mov r12, 28
    int 80h

    pop r12

    mov rsp, rbp
    pop rbp
    ret

callSemState:
    push rbp
    mov rbp, rsp 

    push r12

    mov r12, 29
    int 80h

    pop r12

    mov rsp, rbp
    pop rbp
    ret

change_input:
    push rbp
    mov rbp, rsp 

    push r12
    push r13
    push r15

    mov r12, 30
    mov r13, rdi
    mov r15, rsi
    int 80h
 
    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

change_output:
    push rbp
    mov rbp, rsp 

    push r12
    push r13
    push r15

    mov r12, 31
    mov r13, rdi
    mov r15, rsi
    int 80h
 
    pop r15
    pop r13
    pop r12

    mov rsp, rbp
    pop rbp
    ret

