#include "pipes.h"

static pipe_t pipes[MAX_PIPES]; //de 0 a MAX_PIPES tenemos los puertos de lectura, para escribir sumamos un offset de MAX_PIPES 
static int num_pipes = 0; //numero de pipes inicializados, pueden haber algunos cerrados

static void initPipe(int i);
static void createPipe(int idDestination[2]);
static int verify(int id, int bytes);

void pipeOpen(int idDestination[2]){
    for(int i=0; i < num_pipes; i++){ //busco un pipe vacante
        if(!pipes[i].openRead && !pipes[i].openWrite){//está vacante
            initPipe(i);
            idDestination[0] = i;
            idDestination[1] = i + MAX_PIPES;
            return;
        }
    }

    createPipe(idDestination); //creo un nuevo pipe al final del vector
}

void pipeClose(int id){
    if((id < MAX_PIPES && id >= num_pipes)|| (id >= MAX_PIPES && id >= num_pipes+MAX_PIPES) || id < 0){
        printS("No hay tal pipe\n");
        return;
    }
    (id < MAX_PIPES)? (pipes[id].openRead = FALSE) : (pipes[id].openWrite = FALSE);
    close_mutex(pipes[id].lock); 
    sem_close(pipes[id].semRead);
    sem_close(pipes[id].semWrite);
}

void pipeWrite(int id, char * address, int bytes){
    if(!verify(id, bytes) || id < MAX_PIPES)
        return;
    id -= MAX_PIPES;
    if(id >= MAX_PIPES)
        return; 
    int pos;
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
    if(!verify(id, bytes) || id >= MAX_PIPES)
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
        pipes[i].openRead? printS("OPEN\n") : printS("CLOSED\n");
        printS("Blocked processes: ");
        peekAll(pipes[i].semRead->queue);
        newline();
        printFullLine();
    }
    for(int i=MAX_PIPES; i < num_pipes + MAX_PIPES; i++){
        printS("Pipe: ");
        printDec(i);
        newline();
        printS("Status: ");
        pipes[i].openWrite? printS("OPEN\n") : printS("CLOSED\n");
        printS("Blocked processes: ");
        peekAll(pipes[i].semWrite->queue);
        printFullLine();
    }
}


static void initPipe(int i){
    pipes[i].openRead = TRUE; 
    pipes[i].openWrite = TRUE;
    pipes[i].lock = init_mutex();
    pipes[i].semWrite = sem_init(PIPESIZE);
    pipes[i].semRead = sem_init(0);
    pipes[i].nread = 0;
    pipes[i].nwritten = 0;
}
static void createPipe(int idDestination[2]){
    if(num_pipes >= MAX_PIPES){
        idDestination[0] = -1; //no puedo crear más pipes
        idDestination[1] = -1;
        return;
    }
    int pipeToOpen = num_pipes; //voy a abrir uno en el último lugar disponible
    num_pipes++;
    initPipe(pipeToOpen);
    idDestination[0] = pipeToOpen;
    idDestination[1] = pipeToOpen + MAX_PIPES;
}

static int verify(int id, int bytes){
    if(bytes < 0){
        printS("Error: bytes negativos\n");
        return 0;
    }
    if(id < 0 || (id >= MAX_PIPES && !pipes[id-MAX_PIPES].openWrite) || (id < MAX_PIPES && !pipes[id].openRead)){
        printS("Error: no se puede acceder al pipe ");
        printDec(id);
        newline();
        return 0;
    }
    return 1;
}
