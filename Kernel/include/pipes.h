#ifndef _PIPES_H_
#define _PIPES_H_

#include "mem_manager.h"
#include "video_driver.h"
#include "semaphore.h"

#define PIPESIZE 1024
#define MAX_PIPES 50
#define TRUE 1
#define FALSE 0

typedef struct pipe_t{
    int open;
    mutex_t * lock;
    char data[PIPESIZE];
    unsigned int nwritten;
    unsigned int nread;
}pipe_t;

//consigo un pipe válido y devuelvo su id. Caso contrario devuelvo -1;
int pipeOpen(); 

//cierro el pipe con ese id
void pipeClose(int id);

//escribe bytes desde address en un pipe con determinado id
void pipeWrite(int id, char * address, int bytes); 

//lee bytes desde address en un pipe con determinado id
void pipeRead(int id, char * address, int bytes); 

//muestra el estado de los pipes
void pipeStates();

#endif