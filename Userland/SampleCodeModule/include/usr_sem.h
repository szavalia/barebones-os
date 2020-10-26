#ifndef _USR_SEM_H_
#define _USR_SEM_H_
    //Syscalls
   extern void callSemInit( void ** sem , int value);
   extern void callSemWait( void * sem);
   extern void callSemPost( void * sem);
   extern void callSemState();
   extern void callSemClose(void * sem);
    //funciones wrapper
   void sem_init( void **sem , int value);
   void sem_post( void * sem);
   void sem_wait( void * sem);
   void sem_state();
   void sem_close(void * sem);
#endif