#include <stdint.h>
#include "reg_t.h"

typedef struct process_t{
    char * name;
    uint PID;
    uint priority;
    reg_t registers;
}process_t;