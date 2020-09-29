#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <stdint.h>
#include "reg_t.h"


typedef struct process_t{
    char * name;
    int PID;
    int state; //0->block, 1->listo, 2->killed
    int priority;
    void * stack_start;
    uint64_t base_pointer;
    uint64_t stack_pointer;
}process_t;

uint64_t scheduler( uint64_t stack_pointer );
void switchProcess( uint64_t stackPointer);
void launchProcess( void * process , int argc , char * argv[] , uint64_t stack_pointer );
void processKill( int pid);
void processDump();
void exceptionKill();
uint64_t getBasePointer( void * start);
int fork( uint64_t stack_pointer ); //legacy
int processIsInForeground();
void printGreeting();
int getProcessCount();
#endif