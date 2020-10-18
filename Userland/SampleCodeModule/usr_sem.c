#include "usr_sem.h"

void sem_init( void **sem , int value){
	callSemInit(sem , value );
}

void sem_post( void * sem){
	callSemPost( sem );
}

void sem_wait( void * sem){
	callSemWait(sem);
}