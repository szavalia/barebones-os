#include "process.h"
#include "semaphore.h"
#include "video_driver.h"
#include "cola.h"
#define MAX_SEMS 250
#define MAX_MUTEX MAX_SEMS + 50


static semaphore_t semaphores[MAX_SEMS];
static queueADT freeded_sems;
static mutex_t mutexes[MAX_MUTEX];
int index_mutex=MAX_SEMS;
int index_sem=0;


extern int atomix_add(int value , void * place );
extern void next_round();
extern void stop_interrupts();

void init_sems(){
    for( int i = 0 ; i<MAX_SEMS; i++){
        semaphores[i].index = 1;
        semaphores[i].flag = SEM_CLOSED;
    }
}

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
        //myNice(1);
        next_process();
    }
   // myNice(2);
    atomix_add( -1 , &(mutex->value));
}

void unlock( mutex_t * mutex){
    atomix_add( 1 , &(mutex->value));
}

//Funciones de semáforo

semaphore_t * sem_init( int value ){
    int aux_index;
    if ( value < 0 ){
        value =-value; 
    }
    printS("New sem with value:");
    printDec((long)value);
    newline();
    if( MAX_SEMS > index_sem){
        aux_index = index_sem++;
    }else{
        aux_index= dequeue(freeded_sems);
        if ( aux_index < 0){
            printS("No more free semaphores");
            return NULL;
        }
    }
    semaphores[aux_index].value = value; 
    semaphores[aux_index].flag = SEM_OPENED;
    mutexes[aux_index].value = 1;
    semaphores[aux_index].mutex = &mutexes[aux_index];
   // semaphores[aux_index].queue = create_queue();
    return &semaphores[aux_index];
}

void sem_wait( semaphore_t * sem){
    if (sem_validacion(sem) < 0){
        return;
    }

    lock(sem->mutex);
    while(sem->value<=0){
        unlock(sem->mutex);
        //myNice(1);
        next_process();
        lock(sem->mutex);
    }
    //myNice(2);
    atomix_add(-1 , &(sem->value) );
    unlock(sem->mutex);
}

void sem_post( semaphore_t * sem){
    if (sem_validacion(sem) < 0 ){
        return;
    }

    lock(sem->mutex);
    atomix_add(1, &(sem->value) );
    unlock(sem->mutex);
}
//Es de kernel
void sem_close_index(int index){
    semaphores[index].flag = SEM_CLOSED;
    queue(freeded_sems, index);
}
//
void sem_close(semaphore_t *sem){
     if (sem_validacion(sem) < 0 ){
        return;
    }
    sem->flag = SEM_CLOSED;
    queue(freeded_sems, sem->index);
}

//-------------------------//

void sem_state(){
    printFullLine();
    printS("Estado de los semaforos:");
    printFullLine();
    for( int i= 0 ; i < MAX_SEMS; i++){
        if( semaphores[i].flag == SEM_OPENED){
            printS("Semaforo nro: ");
            printDec(i);
            newline();
            printS("Value: ");
            printDec(semaphores[i].value);
            newline();
            printFullLine();
        }else if( i < index_sem){
            printS("Semaforo nro: ");
            printDec(i);
            newline();
            printS("Este semaforo fue cerrado y esta la queue\n");
            printFullLine();  
        }
    }
}

int sem_validacion( semaphore_t * sem){
    if ( (sem - semaphores) < 0 ){
        return -2;
    }
    if ( (sem - &(semaphores[MAX_SEMS-1])) > 0 ){
        return -2;
    } 
    if ( (sem - semaphores) % sizeof(semaphores) != 0 ){
        return -2;
    }
    if ( sem->flag == SEM_CLOSED){
        return -1;
    }
    return 0;
}

void printAllqueue(){
    int aux ;
    printS("Freeded sems: ");
    while( (aux= dequeue(freeded_sems)) >= 0  ){
        printDec((long)aux);
        printS(" ");
    }
    newline();
}