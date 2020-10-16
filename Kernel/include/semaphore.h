#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
typedef struct mutex_t{
    int value;
}mutex_t;

typedef struct semaphore_t{
    int value;
    mutex_t * mutex;
}semaphore_t;

//funciones mutex
mutex_t * init_mutex();

void lock( mutex_t * mutex);

void unlock( mutex_t * mutex);
//Funciones de semáforo

semaphore_t * sem_init( int value );

void sem_wait( semaphore_t * sem);

void sem_post( semaphore_t * sem);

#endif