#include <stdint.h>
#include "reg_t.h"

typedef struct process_t{
    char * name;
    int PID;
    int priority;
    void * stack_start;
    uint64_t base_pointer;
    uint64_t stack_pointer;
}process_t;