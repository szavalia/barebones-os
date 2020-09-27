#include <stdint.h>
#include "reg_t.h"

typedef struct process_t{
    char * name;
    int PID;
    int estado; //0->block, 1->listo, 2->killed
    int priority;
    void * stack_start;
    uint64_t base_pointer;
    uint64_t stack_pointer;
}process_t;



uint64_t scheduler( uint64_t stack_pointer );
void switchProcess( uint64_t stackPointer);