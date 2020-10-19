#include "usr_sem.h"
#include "usr_lib.h"
#include "usr_strings.h"
#define N 20
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 

#define LEFT (phnum + 1 ) % philo_count 
#define RIGHT (phnum + philo_count - 1 ) % philo_count

#define LEFT_FORK (phnum + 1) % philo_count
#define RIGHT_FORK (phnum ) % philo_count
int philosopher_problem( int argc , char * argv[]);