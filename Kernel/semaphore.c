#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include "process.h"
#include "semaphore.h"
#include "video_driver.h"

#define MAX_SEMS 250
#define MAX_MUTEX MAX_SEMS + 50


static semaphore_t semaphores[MAX_SEMS];
static mutex_t mutexes[MAX_MUTEX];
int index_mutex=MAX_SEMS;
int index_sem=0;


extern int atomix_add(int value , void * place );
extern void next_round();
extern void stop_interrupts();

void next_process(){
    next_round(); 
    stop_interrupts(); //--> kernel
}

//Funciones de mutex
mutex_t * init_mutex(){
    mutexes[index_mutex].value = 1;
    return &mutexes[index_mutex++];
}

void lock( mutex_t * mutex){
    while( mutex->value <=0){   
        myNice(1);
        next_process();
    }
    myNice(2);
    atomix_add( -1 , &(mutex->value));
}

void unlock( mutex_t * mutex){
    atomix_add( 1 , &(mutex->value));
}

//Funciones de semáforo

semaphore_t * sem_init( int value ){
    if ( value < 0 ){
        value =-value; 
    }
    printS("New sem with value:");
    printDec((long)value);
    newline();
    semaphores[index_sem].value = value; 
    mutexes[index_sem].value = 1;
    semaphores[index_sem].mutex = &mutexes[index_sem];
    printHex(&semaphores[index_sem]);
    newline();
    return &semaphores[index_sem++];
}

void sem_wait( semaphore_t * sem){
    lock(sem->mutex);
    while(sem->value<=0){
        unlock(sem->mutex);
        myNice(1);
        next_process();
        lock(sem->mutex);
    }
    myNice(2);
    atomix_add(-1 , &(sem->value) );
    unlock(sem->mutex);
}

void sem_post( semaphore_t * sem){
    lock(sem->mutex);
    atomix_add(1, &(sem->value) );
    unlock(sem->mutex);
}


