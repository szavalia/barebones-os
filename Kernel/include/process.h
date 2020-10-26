#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <stdint.h>
#include "reg_t.h"
#include "cola.h"
#include "pipes.h"
#include "lib.h"
#include "moduleLoader.h"
#include "video_driver.h"
#include "mem_manager.h"

#define STACK_SIZE ( 32 * 1024 + 8 )
#define MAXPROCESOS 50
#define BASE_PRIORITY 1
#define MAX_PRIORITY 4
#define STACK_ALIGN(number)  number & -32 
#define NOT_CREATED 0
#define READY 1
#define BLOCKED 2
#define KILLED 3
#define TRUE 1
#define FALSE 0
#define STDIN 0
#define STDOUT MAX_PIPES
typedef struct process_t{
    char * name;
    int PID;
    int state; //0->block, 1->listo, 2->killed
    int priority;
    int ticks_left;
    void * stack_start;
    uint64_t base_pointer;
    uint64_t stack_pointer;
    int pipes[2]; //pipes[0] es el pipe de lectura, pipes[1] el de escritura
}process_t;

uint64_t scheduler( uint64_t stack_pointer );
void switchProcess( uint64_t stackPointer);
void launchProcess( void * process , int argc , char * argv[], int * pid_destination , uint64_t stack_pointer );
void processKill( int pid);
void processNice(int pid, int new_prio);
void processDump();
uint64_t getBasePointer( void * start);
int processIsInForeground();
int getPID();
void printGreeting();
int getProcessCount();
void exitProcess();
void processBlock(int pid);
int currentPID();
void unblockByQueue( queueADT queue);

void change_input(int pipeID, int pid);
void change_output(int pipeID, int pid);
int getWritePipe();
int getReadPipe();

extern void renounce();
#endif