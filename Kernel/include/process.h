#include <stdint.h>
#include "reg_t.h"

typedef struct process_t{
    char * name;
    int PID;
    int priority;
    uint64_t stack_base;
    reg_t registers;
}process_t;