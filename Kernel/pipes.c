#include "pipes.h"

static pipe_t pipes[MAX_PIPES]; //cada pipe tiene un id, le pongo -1 si fue cerrado
static int num_pipes = 0; //numero de pipes inicializados, pueden haber algunos cerrados

static void initPipe(int i);
static int createPipe();
static int verify(int id, int bytes);

int pipeOpen(){
    for(int i=0; i < num_pipes; i++){ //busco un pipe vacante
        if(!pipes[i].open){//está vacante
            initPipe(i);
        }
    }
    return createPipe();
}

void pipeClose(int id){
    if(id >= num_pipes){
        printS("No hay tal pipe\n");
        return;
    }
    pipes[id].open = FALSE;
    //close_mutex(pipes[id].lock); //TODO: implementar close_mutex
}

void pipeWrite(int id, char * address, int bytes){
    if(!verify(id, bytes))
        return;

    int pos;
    for(int i = 0; i < bytes; i++){
        while(1){
            lock(pipes[id].lock);
            if(pipes[i].nwritten < pipes[id].nread){
                break;
            }
            unlock(pipes[id].lock);
        }
        pos = pipes[id].nwritten++;
        pipes[id].data[pos%PIPESIZE] = address[i];
        unlock(pipes[id].lock);
    }

}

void pipeRead(int id, char * address, int bytes){
    if(!verify(id, bytes))
        return;    
        
    int pos;
    for(int i = 0; i<bytes; i++){
        while(1){
            lock(pipes[id].lock);
            if(pipes[i].nread < pipes[id].nwritten){
                break;
            }
            unlock(pipes[id].lock);
        }
        pos = pipes[id].nread++;
        address[i] = pipes[id].data[pos%PIPESIZE];
        unlock(pipes[id].lock);
    }
}

static void initPipe(int i){
    pipes[i].open = TRUE;
    pipes[i].lock = init_mutex();
    pipes[i].nread = 0;
    pipes[i].nwritten = 0;
}
static int createPipe(){
    if(num_pipes >= MAX_PIPES){
        return -1; //no puedo crear más pipes
    }
    int pipeToOpen = num_pipes; //voy a abrir uno en el último lugar disponible
    num_pipes++;
    initPipe(pipeToOpen);
    return pipeToOpen;
}

static int verify(int id, int bytes){
    if(bytes < 0){
        printS("Error: bytes negativos\n");
        return 0;
    }
    if(id < 0 || id >=num_pipes || !pipes[id].open){
        printS("Error: no hay tal pipe\n");
        return 0;
    }
    return 1;
}
