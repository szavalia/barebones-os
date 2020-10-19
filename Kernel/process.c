#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include "process.h"
#include "reg_t.h"
#include "video_driver.h"
#include "mem_manager.h"
#include "cola.h"

#define STACK_SIZE ( 32 * 1024 + 8 )
#define MAXPROCESOS 50
#define BASE_PRIORITY 1
#define MAX_PRIORITY 4
#define STACK_ALING(number)  number & -32 
#define NULL 0 
#define NOT_CREATED 0
#define READY 1
#define BLOCKED 2
#define KILLED 3
#define TRUE 1
#define FALSE 0
#define STDIN 0
#define STDOUT 250

long process_count = 0; //todos los procesos que alguna vez estuvieron activos
int active_processes = 0; //procesos bloqueados o ready
int initialized = FALSE;
void * entryPoint;
static process_t procesos[MAXPROCESOS];
int current_proc = 0, foreground_proc = -1;
extern int side, context;
char kernelName[] = "Kernel";
char unnamed[] = "Unnamed";
static queueADT freed_pids;

extern void prepareProcess( int PID , uint64_t stackPointer , int argc , char * argv[] , void * main);
extern void switchProcess( uint64_t stackPointer);
extern void prepareProcessForked(uint64_t stack_pointer , uint64_t basePointerParent , uint64_t basePointerChild );


int getProcessCount(){
    return active_processes;
}

void * requestStack(){
    return ltmalloc( STACK_SIZE );
}

void processBlock(int pid){
    if(pid <= 0 || pid > MAXPROCESOS){
        printS("Error en el PID\n");
        return;
    }
    if(procesos[pid].state == BLOCKED){
        procesos[pid].state = READY;
    }
    else if( procesos[pid].state == READY){
        procesos[pid].state = BLOCKED;
    }
    return;
}


void printState( int i ){
    printS("\nState: ");
    switch (i)
    {
    case READY:
        printS("READY");
        break;
    
    case BLOCKED:
        printS("BLOCKED");
        break;
    case KILLED:
        printS("KILLED");
        break;
    default:
        printS("UNDEFINED");
        break;
    }
}

void processNice(int pid, int new_prio){ 
    if(new_prio < BASE_PRIORITY){
        return;
    }
    if(new_prio > MAX_PRIORITY){
        new_prio = MAX_PRIORITY;
    }

    if(procesos[pid].state != NOT_CREATED || procesos[pid].state != KILLED){
        procesos[pid].priority = new_prio;
        procesos[pid].ticks_left = new_prio-1;
        return;        
    }
}

void myNice(int new_prio){
    processNice(current_proc, new_prio);
}

void processDump(){
    int i;
    printS("Procesos Activos:\n");
    for( i = 0 ; i < MAXPROCESOS ; i++){
        if( procesos[i].state != NOT_CREATED ){
        printFullLine();
        printS("proceso: ");
        printS(procesos[i].name);
        printS("\nPID:");
        printDec(procesos[i].PID);
        printS("\nPriority: ");
        printDec(procesos[i].priority);
        printS("\nTicks left: ");
        printDec(procesos[i].ticks_left);
        printState(procesos[i].state);
        printS("\nForeground: ");
        if( procesos[i].PID == foreground_proc){
            printS("yes");
        }
        else{
            printS("no");
        }
        printS("\nBP: ");
        printHex(procesos[i].base_pointer);
        printS("\nSP: ");
        printHex(procesos[i].stack_pointer);
        newline();
        }
    }
    printFullLine();
}

uint64_t getBasePointer( void * start){
    uint64_t stack = STACK_ALING( (int) start); // 32 kb de stack --> alineado mas atras pierdo hasta 7 bytes, lo que lo compenso agregando 1 cuando pido memoria 
    stack+= STACK_SIZE; // llevo el pointer al final, la cantidad de stack seguro es de 32kb;
    return stack;
}

int createPID(){
    int pid;
    if ( process_count >= MAXPROCESOS ){
        pid = dequeue(freed_pids);
        if ( pid < 0){
            printS("NO AVAILABLE PIDS\n");
            return -1;
        }
    }else
    {
        pid = process_count++;
    }
    active_processes++;
    return pid;
}
void restart_kernel(){
    if(active_processes <= 1){
        printS("[ Restarting Kernel ]\n");
        for ( int i = 0 ; i < MAXPROCESOS ; i++){
          if (procesos[i].state != NOT_CREATED){
              ltmfree(procesos[i].stack_start);
          }
          procesos[i].state = NOT_CREATED;
        }
        freeQueue(freed_pids);
        process_count = 0;
        active_processes = 0;
        launchProcess(entryPoint , 0 , 0 , 0);
    }    
}

void launchProcess( void * process , int argc , char **argv, int * pid_destination , uint64_t stack_pointer ){

    int pid = createPID();
    *pid_destination = pid;

        for ( int i = 0 ; i < MAXPROCESOS ; i++){
            procesos[i].state = NOT_CREATED;
        }
        entryPoint = process;
        initialized = 1;
        freed_pids = create_queue();
    }
    procesos[current_proc].stack_pointer = stack_pointer;
    current_proc = pid;
    procesos[pid].PID= pid;
    if ( argc != 0 ){
        procesos[pid].name = *argv;
    }
    else
    {
        procesos[pid].name = unnamed;
    }
    procesos[pid].state = READY;

    if( pid != 0){
        if(foreground_proc < 0){ //si no hay nadie en foreground, tomalo
            foreground_proc = current_proc;
        procesos[pid].pipes[0] = STDIN;
        }
        procesos[pid].priority = BASE_PRIORITY;
        procesos[pid].ticks_left = BASE_PRIORITY-1;
    }else{
        procesos[pid].priority = 1;
        procesos[pid].ticks_left = 1;
    }
    procesos[pid].piprs[1] = STDOUT; //todos tienen como default escribir en la terminal
    

    procesos[pid].stack_start = requestStack(); 
    procesos[pid].base_pointer = getBasePointer(procesos[pid].stack_start);
    procesos[pid].stack_pointer = procesos[pid].base_pointer;    

    prepareProcess(pid , procesos[pid].base_pointer , argc , argv , process);
}



uint64_t scheduler (uint64_t current_rsp){
    procesos[current_proc].stack_pointer = current_rsp; 
    if( procesos[current_proc].state == READY){
        if(procesos[current_proc].ticks_left > 0){ //para procesos con prioridades mayores a 1
            procesos[current_proc].ticks_left--;
            return procesos[current_proc].stack_pointer;
        }
    }
    procesos[current_proc].ticks_left = procesos[current_proc].priority-1;    
    int i = current_proc;
    do{
        i++;
        if(i == MAXPROCESOS){
            i = 0;
        }        
    }
    while(procesos[i].state != READY );
    current_proc = i;
    if(foreground_proc == -1){
        foreground_proc = current_proc;
    }
    
    return procesos[i].stack_pointer;
}

void exitProcess(){
    
    procesos[current_proc].state = NOT_CREATED;
    queue(freed_pids , current_proc);
    ltmfree(procesos[current_proc].stack_start);
    active_processes--;
    restart_kernel();
    
    if(foreground_proc == current_proc){
        foreground_proc = -1;
    }
    switchProcess(scheduler(procesos[current_proc].stack_pointer));
    
}

void processKill( int pid){
    
    if(foreground_proc == pid){
        foreground_proc = -1;
    }
    if(pid ==procesos[current_proc].PID){
        exitProcess();
    }
    if (  procesos[pid].state == READY || procesos[pid].state == BLOCKED){
        procesos[pid].state = KILLED;
        ltmfree(procesos[pid].stack_start);
        queue(freed_pids, pid);
        active_processes--;
        restart_kernel();
        
    }else{
        printS("No such process is alive\n");
    }
    
}


int processIsInForeground(){
    return current_proc == foreground_proc;
}

void printGreeting(){
    printS("Hola! Soy el proceso con PID ");
    printDec(procesos[current_proc].PID);
    newline();       
}

int getPID(){
    return procesos[current_proc].PID;
}


void unblockByQueue( queueADT queue){
    int aux;
    int *vec;
    while (( aux=dequeue(queue)) > 0 ){
        
        if ( procesos[aux].state == BLOCKED){
            procesos[aux].state = READY;
        }
    }
}

void change_input(int pid, int pipeID){
    if(pid <= 0 || pid > process_count){
        return;
    }
    procesos[pid].pipes[0] = pipeID;
}

void change_input(int pid, int pipeID){
    if(pid <= 0 || pid > process_count)
        return;
    }
    procesos[pid].pipes[1] = pipeID;
}