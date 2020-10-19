#include "usr_sem.h"
#include "usr_lib.h"
#include "usr_strings.h"
#define N 20
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 

#define LEFT (phnum + 1 ) % N 
#define RIGHT (phnum + N - 1 ) % N 

#define LEFT_FORK (phnum + 1) % N 
#define RIGHT_FORK (phnum ) % N 
int philosopher_problem( int argc , char * argv[]);