#include "process.h"
#include "semaphore.h"
#include "video_driver.h"
#include "cola.h"
#include "mem_manager.h"
#define MAX_SEMS 250
#define MAX_MUTEX MAX_SEMS + 50

static semaphore_t semaphores[MAX_SEMS];
static queueADT freeded_sems;
static mutex_t mutexes[MAX_MUTEX];
int index_mutex=MAX_SEMS;
int index_sem=0;

int sem_validacion( semaphore_t * sem);
extern int atomix_add(int value , void * place );
extern void next_round();
extern void stop_interrupts();

void init_sems(){
    for( int i = 0 ; i<MAX_SEMS; i++){
        semaphores[i].index = 1;
        semaphores[i].flag = SEM_CLOSED;
        mutexes[i].index = i;
        mutexes[i].flag = MUT_CLOSED;
    }

}

void block_me(queueADT q){
    int pid = getPID();
    queue(q, pid);
    blockProcess(pid);
}

void next_process( queueADT queue){
    block_me(queue);
    next_round(); 
    stop_interrupts(); //--> kernel
}

//Funciones de mutex 

mutex_t * init_mutex(){
    mutexes[index_mutex].value = 1;
    mutexes[index_mutex].flag = MUT_OPENED;
    mutexes[index_mutex].queue = create_queue;
    return &mutexes[index_mutex++];
}

void lock( mutex_t * mutex){
    while( mutex->value <=0){   
        //myNice(1);
        next_process( mutex->queue);
    }
   // myNice(2);
    atomix_add( -1 , &(mutex->value));
}

void unlock( mutex_t * mutex){
    atomix_add( 1 , &(mutex->value));
    unblockByQueue(mutex->queue);
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
    mutexes[aux_index].flag = MUT_OPENED;
    semaphores[aux_index].mutex = &mutexes[aux_index];
    semaphores[aux_index].queue = create_queue();
    mutexes[aux_index].queue = create_queue();
    return &semaphores[aux_index];
}

void sem_wait( semaphore_t * sem){
    if (sem_validacion(sem) < 0){
        return;
    }

    lock(sem->mutex);
    while(sem->value<=0){
        unlock(sem->mutex);
        next_process(sem->queue);
        lock(sem->mutex);
    }
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
    unblockByQueue(sem->queue);
}
//Es de kernel
void sem_close_index(int index){
    if ( peek(semaphores[index].queue)>0){
    semaphores[index].flag = SEM_CLOSED;
    queue(freeded_sems, index);
    }else
    {
        printS("El semaforo tiene cosas en espera, falló el cerrado");
    }
    
}
//
void sem_close(semaphore_t *sem){
     if (sem_validacion(sem) < 0 ){
        return;
    }
    if( peek(sem->queue) < 0 ){
        printS("El semaforo tiene cosas en espera, falló el cerrado");
        return;
    }
    sem->flag = SEM_CLOSED;
    queue(freeded_sems, sem->index);
}

//-------------------------//

void sem_state(){
    int **vector;
    printFullLine();
    printS("Estado de los semaforos:");
    printFullLine();
    int j;
    for( int i= 0 ; i < MAX_SEMS; i++){
        if( semaphores[i].flag == SEM_OPENED){
            printS("Semaforo nro: ");
            printDec(i);
            newline();
            printS("Value: ");
            printDec(semaphores[i].value);
            newline();
            if ( peek(semaphores[i].queue) >= 0 ){
                printS("Con los siguientes procesos en cola:");
                peekAll(semaphores[i].queue, vector);
                j=0;
                while((*vector)[j] != -1){
                    printDec((long)(*vector)[j++]);
                    printS(" ");
                }
                ltmfree(*vector);
            }
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
