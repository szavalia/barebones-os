#include "pipes.h"

static pipe_t pipes[MAX_PIPES]; 
static int num_pipes = 0; //numero de pipes inicializados, pueden haber algunos cerrados

static void initPipe(int i);
static int createPipe();
static int verify(int id, int bytes);

int pipeOpen(){
    for(int i=0; i < num_pipes; i++){ //busco un pipe vacante
        if(!pipes[i].open){//está vacante
            initPipe(i);
            return i;
        }
    }

    return createPipe();
}

void pipeClose(int id){
    if(id >= num_pipes || id < 0){
        printS("No hay tal pipe\n");
        return;
    }
    pipes[id].open = FALSE;
    //close_mutex(pipes[id].lock); //TODO: sacar comentario cuando esté implementado
    sem_close(pipes[id].semRead);
    sem_close(pipes[id].semWrite);
}

void pipeWrite(int id, char * address, int bytes){
    if(!verify(id, bytes))
        return;

    int pos, bytesWritten=0;
    for(int i=0; i < bytes; i++){
        sem_wait(pipes[id].semWrite); //pido lugar para escribir
        lock(pipes[id].lock);

        pos = pipes[id].nwritten++;
        pipes[id].data[pos%PIPESIZE] = address[i];
        sem_post(pipes[id].semRead); //agrego un lugar para leer
        
        unlock(pipes[id].lock);        
    }  
}

void pipeRead(int id, char * address, int bytes){
    if(!verify(id, bytes))
        return;    
        
    int pos;
    for(int i = 0; i<bytes; i++){
        sem_wait(pipes[id].semRead); //pido un lugar para leer
        
        lock(pipes[id].lock);

        pos = pipes[id].nread++;
        address[i] = pipes[id].data[pos%PIPESIZE];
        sem_post(pipes[id].semWrite); //agrego un lugar para escribir

        unlock(pipes[id].lock);
    }
}

void pipeStates(){
    if(num_pipes == 0){
        printS("No abriste ningun pipe\n");
    }
    for(int i=0; i< num_pipes; i++){
        printS("Pipe: ");
        printDec(i);
        newline();
        printS("Status: ");
        pipes[i].open? printS("OPEN\n") : printS("CLOSED\n");
        printS("Chars written: ");
        printDec(pipes[i].nwritten);
        newline();
        printS("Chars read: ");
        printDec(pipes[i].nread);
        newline();
        printFullLine();
    }
}


static void initPipe(int i){
    pipes[i].open = TRUE;
    pipes[i].lock = init_mutex();
    pipes[i].semWrite = sem_init(PIPESIZE);
    pipes[i].semRead = sem_init(0);
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
        printS("Error: no se puede acceder al pipe\n");
        return 0;
    }
    return 1;
}
