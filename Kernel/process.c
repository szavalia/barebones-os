#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include "process.h"
#include "kernel.c"
#include "reg_t.h"
#define MAXPROCESOS 250
#define BASE_PRIORITY 0
long process_count = 1; 

process_t procesos[MAXPROCESOS];

char kernelName[] = "Kernel";




uint64_t getBasePointer(){
    //aca tenemos que crear los stacks de los programas... Juega con el memory manager no?
    return stackBase;
}


int createPID(){
    if ( process_count >=250 ){
        return -1;
    }
    return process_count++;


}

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



//fork
int fork( int fatherPID ){
    //un fork es agarrar un proceso, ponerle un pid y copiar el resto de los otros.
    int pid = createPID();
    if ( pid = -1 ){
        //Error
    }
    procesos[pid].PID = pid;
    procesos[pid].registers = procesos[fatherPID].registers;
    procesos[pid].priority= BASE_PRIORITY;

}

//execvec
void launchProcess( void * process , int argc , char * args[]  ){
    //Deberia ser como un execve esto, el void* del procesos seria onda (entryPoint);
    //Igual siento que esto tendria que usarse el conjunto el fork en userland, porque pisaria el 
    int pid = createPID();

}