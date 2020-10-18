#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_


typedef struct mutex_t{
    int value;
}mutex_t;

typedef struct semaphore_t{
    int flag;
    int index;
    int value;
    mutex_t * mutex;
}semaphore_t;

#define SEM_OPENED 1
#define SEM_CLOSED 0

//funciones mutex
mutex_t * init_mutex();

void lock( mutex_t * mutex);

void unlock( mutex_t * mutex);
//Funciones de semáforo

semaphore_t * sem_init( int value );

void sem_wait( semaphore_t * sem);

void sem_post( semaphore_t * sem);

void sem_state();
void sem_close_index(int index);
void init_sems();

void printAllqueue();
#endif