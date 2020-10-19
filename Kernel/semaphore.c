#include "process.h"
#include "semaphore.h"
#include "video_driver.h"
#include "cola.h"
#include "mem_manager.h"
#include "pipes.h"
#define MAX_SEMS MAX_PIPES * 2 + 200 // Al menos un semaforo para cada pipe, y 200 semaforos libres
#define MAX_MUTEX MAX_SEMS + 50

static semaphore_t semaphores[MAX_SEMS];
static queueADT freed_sems;
int index_sem=0;


static mutex_t mutexes[MAX_MUTEX];
static queueADT freed_mutex;
int index_mutex=MAX_SEMS; //Los primeros mutexes corresponden a semaforos


int sem_validacion( semaphore_t * sem);
extern int atomix_add(int value , void * place );
extern void next_round();
extern void stop_interrupts();

void init_sems(){
    int i;
    freed_sems = create_queue();
    freed_mutex = create_queue();
    for( i = 0 ; i<MAX_SEMS; i++){
        semaphores[i].index = 1;
        semaphores[i].flag = SEM_CLOSED;
        mutexes[i].index = i;
        mutexes[i].flag = MUT_CLOSED;
    }
}

void block_me(queueADT q){
    int pid = getPID();   
    queue(q, pid);
    processBlock(pid);
}

void next_process( queueADT queue){
    block_me(queue);
    next_round();
    stop_interrupts(); 
   
}

//Funciones de mutex 

mutex_t * init_mutex(){
    int index;
    if ( index_mutex < MAX_MUTEX ){
        index = index_mutex++;
    }else{
        index = dequeue(freed_mutex);
        if ( index < 0 ){
            printS("No more free mutexes");
            return NULL;
        }
    }
    mutexes[index].index = index;
    mutexes[index].value = 1;
    mutexes[index].flag = MUT_OPENED;
    mutexes[index].queue = create_queue();
    return &mutexes[index];
}

void lock( mutex_t * mutex){
    if ( mutex_validacion(mutex) < 0 ){
        return;
    }
    while( mutex->value <=0){   
        next_process( mutex->queue);
    }
    atomix_add( -1 , &(mutex->value));
}

void unlock( mutex_t * mutex){
    if ( mutex_validacion(mutex) < 0 ){
        return;
    }
    atomix_add( 1 , &(mutex->value));
    unblockByQueue(mutex->queue);
}

void close_mutex(mutex_t * mutex){
    if ( mutex_validacion(mutex) < 0 ){
        return;
    }
    if( peek(mutex->queue)< 0 ){
        printS("El mutex tienecosas en espera, fallo el cerrado");
    }
    mutex->flag=MUT_CLOSED;
    queue(freed_mutex, mutex->index);
}
//Funciones de semáforo

semaphore_t * sem_init( int value ){
    int aux_index;
    if ( value < 0 ){
        value =-value; 
    }
    if( MAX_SEMS > index_sem){
        aux_index = index_sem++;
    }else{
        aux_index= dequeue(freed_sems);
        if ( aux_index < 0){
            printS("No more free semaphores");
            return NULL;
        }
    }
    semaphores[aux_index].value = value; 
    semaphores[aux_index].flag = SEM_OPENED;
    semaphores[aux_index].mutex = &mutexes[aux_index];
    semaphores[aux_index].queue = create_queue();

    mutexes[aux_index].value = 1;
    mutexes[aux_index].flag = MUT_OPENED;
    mutexes[aux_index].queue = create_queue();
    printS(" ");
    printHex(&(semaphores[aux_index]));
    printS(" ");
    return &(semaphores[aux_index]);
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

    
    unblockByQueue(sem->queue);
    unlock(sem->mutex);
  
}
//Es de kernel
void sem_close_index(int index){
    if ( peek(semaphores[index].queue) >0 ){
        semaphores[index].flag = SEM_CLOSED;
        queue(freed_sems, index);
    }else{
        printS("El semaforo tiene cosas en espera, fallo el cerrado");
    }
    
}
void sem_close(semaphore_t *sem){
     if (sem_validacion(sem) < 0 ){
        return;
    }
    if( peek(sem->queue) < 0 ){
        printS("El semaforo tiene cosas en espera, fallo el cerrado");
        return;
    }
    sem->flag = SEM_CLOSED;
    queue(freed_sems, sem->index);
}

//-------------------------//

void sem_state(){
    int **vector;
    printFullLine();
    printS("Estado de los semaforos\n");
    printFullLine();
    //int j;
    for( int i= 0 ; i < MAX_SEMS; i++){
        if( semaphores[i].flag == SEM_OPENED){
            printS("Semaforo nro: ");
            printDec(i);
            newline();
            printS("Value: ");
            printDec(semaphores[i].value);
            newline();
            if ( peek(semaphores[i].queue) >= 0 ){
                printS("Con los siguientes procesos en cola del sem:");
                peekAll(semaphores[i].queue, vector);
                /*j=0;
                while((*vector)[j] != -1){
                    printDec((long)(*vector)[j++]);
                    printS(" ");
                }
                ltmfree(*vector);*/
            }
            if ( peek(semaphores[i].mutex->queue) >= 0 ){
                printS("Con los siguientes procesos en cola del mutex:");
                peekAll(semaphores[i].mutex->queue, vector);
                /*j=0;
                while((*vector)[j] != -1){
                    printDec((long)(*vector)[j++]);
                    printS(" ");
                }
                ltmfree(*vector);*/
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


int mutex_validacion( mutex_t * mutex){
    int error=0;
    void * mut1 = mutex;
    void * mutInitial = mutexes;
    void * mutFinal = &(mutexes[MAX_MUTEX-1]);
    if ( (mut1 - mutInitial) < 0 ){
        printS("pointer too small");
        error= -2;
    }else if ( (mut1 - mutFinal) > 0 ){
        printS("pointer too big");
        error=-2;
    } else if ( (mut1 - mutInitial) % sizeof(mutex_t) != 0 ){
            printS("pointer not alligned\n");
            error = -2;
    }else if ( mutex->flag == MUT_CLOSED){
        printS("mut closed");
        error = -1;
    }
    return error;
}


int sem_validacion( semaphore_t * sem){
    int error=0;
    void * sem1 = &semaphores[1];
    void * sem2 = &semaphores[2];
    void * semInit = sem;
    void * semMax = &(semaphores[MAX_SEMS-1]);
    if ( (sem - semaphores) < 0 ){
        printS("pointer too small");
        error= -2;
    }else if ( (semInit - semMax) > 0 ){
        printS("pointer too big: ");
        printHex(semInit);
        printS(" max :");
        printHex(semMax);
        error=-2;
    } else if ( ((long)sem - (long)semaphores) % sizeof(semaphore_t) != 0 ){
            printDec(sizeof(semaphore_t));
            newline();
            printDec(((long)sem - (long)semaphores) % sizeof(semaphore_t) );
            printS("NO ES UNO\n");
            error = -2;
    }else if ( sem->flag == SEM_CLOSED){
        printS("Sem closed");
        error = -1;
    }
    return error;
}
