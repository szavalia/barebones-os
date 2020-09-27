#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include "process.h"
#include "reg_t.h"
#include "video_driver.h"
#define MAXPROCESOS 50
#define BASE_PRIORITY 0
#define STACK_ALING(number)  number & -32 
#define NULL 0 
#define NOT_CREATED 0
#define READY 1
#define BLOCKED 2
#define KILLED 3
long process_count = 0; 
int actual = 1;
int flag = 0;
void * secondmain;
process_t procesos[MAXPROCESOS];
int current_proc = 0;

char kernelName[] = "Kernel";
char unnamed[] = "Unnamed";
extern void prepareProcess( int PID , uint64_t stackPointer , int argc , char * argv[] , void * main);
extern void switchProcess( uint64_t stackPointer);

void * requestStack(){
    return ltmalloc( 32 * 1024 + 8 );
}


/*void test(){  
    while(1){
        printS("test");
        newline();
        };
}

uint64_t scheduler( uint64_t stack_pointer ){
    if ( flag == 0 ){
        flag = 1;
        procesos[1].stack_pointer = stack_pointer;
        actual = 2;
        launchProcess( test , 0 , 0 );
    }
    else{
        if ( actual == 2  ){
            actual = 1;
            procesos[2].stack_pointer = stack_pointer;
            return procesos[1].stack_pointer;
        }
        else{
            actual = 2;
            procesos[1].stack_pointer= stack_pointer;
            return procesos[2].stack_pointer;
        }
    }
}*/



uint64_t getBasePointer( void * start){
    uint64_t stack = STACK_ALING( (int) start); // 32 kb de stack --> alineado mas atras pierdo hasta 7 bytes, lo que lo compenso agregando 1 cuando pido memoria 
    stack+= 32*1024 + 8; // llevo el pointer al final, la cantidad de stack seguro es de 32kb;
    return stack;
}

int createPID(){
    if ( process_count >=250 ){
        return -1;
    }
    process_count+=1;
    return process_count;


}
/*
void kernelLaunch(){
    procesos[0].PID = 0;
    procesos[0].registers; //No se que poner aca
    procesos[0].name = kernelName;
    procesos[0].priority = 1;
}

void fillRegisters( process_t *proceso , reg_t *registers ){ //recibe un proceso, podria hacerse con el PID, seria mas lindo no?
    proceso->registers.r8 = registers->r8;
    proceso->registers.r9 = registers->r9 ;
    proceso->registers.r10 = registers->r10;
    proceso->registers.r11 = registers->r11;
    proceso->registers.r12 = registers->r12;
    proceso->registers.r13 = registers->r13;
    proceso->registers.r14 = registers->r14;
    proceso->registers.r15 = registers->r15;
    proceso->registers.rax = registers->rax;
    proceso->registers.rbx = registers->rbx;
    proceso->registers.rcx = registers->rcx;
    proceso->registers.rdx = registers->rdx;
    proceso->registers.rdi = registers->rdi;
    proceso->registers.rsi = registers->rsi;
    proceso->registers.rbp = registers->rbp;
    proceso->registers.rsp = registers->rsp;
    proceso->registers.rip = registers->rip;
    proceso->registers.cs = registers->cs;
    proceso->registers.flags = registers->flags; 
}
*/

//fork
int fork( int fatherPID ){
    //un fork es agarrar un proceso, ponerle un pid y copiar el resto de los otros.
    int pid = createPID();
    if ( pid = -1 ){
        //Error
    }
    procesos[pid].PID = pid;
    //procesos[pid].registers = procesos[fatherPID].registers;
    procesos[pid].priority= BASE_PRIORITY;

}

//execvec
void launchProcess( void * process , int argc , char * argv[]  ){
    //Deberia ser como un execve esto, el void* del procesos seria onda (entryPoint);
    //Igual siento que esto tendria que usarse el conjunto el fork en userland, porque pisaria el 
    int pid = createPID();
    procesos[pid].PID= pid;
    if ( argv != 0 ){
        procesos[pid].name = argv[0];
    }
    else
    {
        procesos[pid].name = unnamed;
    }
    procesos[pid].state = READY;
    procesos[pid].stack_start = requestStack();
    procesos[pid].base_pointer = getBasePointer(procesos[pid].stack_start);
    printHex(procesos[pid].base_pointer);
    prepareProcess(pid , procesos[pid].base_pointer , argc , argv , process);
}



uint64_t scheduler (uint64_t current_rsp){
    procesos[current_proc].stack_pointer = current_rsp;
    int i = current_proc;
    do{
        i++;
        if(i ==MAXPROCESOS){
            i = 0;
        }
    }
    while(procesos[i].state != READY );
    current_proc = i;
    return procesos[i].stack_pointer;
}

void processKill( int pid){
    if (  procesos[pid].state != NOT_CREATED ){
        procesos[pid].state = KILLED;
    }else
    {
        printS("No such process\n");
    }
    
}
void processBlock( int pid){
    if (  procesos[pid].state == READY){
        procesos[pid].state = BLOCKED;
    }else
    {
        printS("No such process \n");
    }
    
}
void printState( int i ){
    printS("State: ");
    switch (i)
    {
    case READY:
        printS("READY\n");
        break;
    
    case BLOCKED:
        printS("BLOCKED\n");
        break;
    case KILLED:
        printS("KILLED\n");
        break;
    default:
        printS("UNDEFINED\n");
        break;
    }
}

void processNice(){
    printS("Unused");
}
void processDump(){
    int i;
    printS("Procesos Activos:\n");
    for( i = 0 ; i < MAXPROCESOS ; i++){
        if( procesos[i].state != NOT_CREATED ){
        printS("proceso: ");
        printS(procesos[i].name);
        printS("\n PID:");
        printDec(procesos[i].PID);
        printS("\nPriority: ");
        printHex(procesos[i].priority);
        printS("\n");
        printState(procesos[i].state);
        printS("\nForeground: ");
        if( procesos[i].foreground == 0){
            printS("no");
        }
        else{
            printS("yes");
        }
        printS("\nBP: ");
        printHex(procesos[i].base_pointer);
        printS("\nSP: ");
        printHex(procesos[i].stack_pointer);
        }
    }
}