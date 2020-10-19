// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "user_interrupts.h"


int int80_handler( uint64_t * stack_pointer){
    int option = stack_pointer[R12];
    switch(option){
        case 0:
            sys_read( stack_pointer);
            break;
        case 1:
            sys_write(stack_pointer);
            break;
        case 2:
            sys_getReg(stack_pointer);
            break;
        case 3:
            sys_time(stack_pointer);
            break;
        case 4:
            sys_getMem(stack_pointer);
            break;
        case 5:
            sys_cpuinfo(stack_pointer);
            break;
        case 6:
            sys_temp(stack_pointer);
            break;
        case 7:
            sys_context(stack_pointer);
            break;
        case 8:
            sys_update_context(stack_pointer);
            break;
        case 9:
            sys_malloc(stack_pointer);
            break;
        case 10:
            sys_free(stack_pointer);
            break;
        case 11:
            sys_mem(stack_pointer);
            break;
        case 12:
            sys_ps(stack_pointer);
            break;
        case 13:
            sys_kill(stack_pointer);
            break;
        case 14:
            sys_launch(stack_pointer);
            break;
        case 15:
            sys_pid(stack_pointer);
            break;
        case 16:
            sys_loop(stack_pointer);
            break;
        case 17:
            sys_exit(stack_pointer);
            break;
        case 18:
            sys_sem_init(stack_pointer);
            break;
        case 19:
            sys_sem_wait(stack_pointer);
            break;
        case 20:
            sys_sem_post(stack_pointer);
            break;
        case 21:
            sys_renounce(stack_pointer);
            break;
        case 22:
            sys_block(stack_pointer);
            break;
        case 23: 
            sys_nice(stack_pointer);
            break;
        case 24:
            sys_pipe_write(stack_pointer);
            break;
        case 25:
            sys_pipe_read(stack_pointer);
            break;
        case 26:
            sys_pipe_open(stack_pointer);
            break;
        case 27:
            sys_pipe_close(stack_pointer);
            break;
        case 28:
            sys_pipe_states(stack_pointer);
            break;
        case 29:
            sys_sem_state(stack_pointer);
            break;    
        case 30:
            sys_change_input(stack_pointer);
            break;
        case 31:
            sys_change_output(stack_pointer);
            break;    

    }
    return 1;
}

void sys_sem_init(uint64_t  regs[] ){
    void ** sem_pointer = (void **) regs[R13];
    *sem_pointer = sem_init(regs[R15]);
    return;
}

void sys_sem_wait(uint64_t  regs[] ){
    sem_wait((semaphore_t *) regs[R13]);
    return;
}
void sys_sem_post(uint64_t  regs[] ){
     sem_post((semaphore_t *)regs[R13]);
    return;
}
void sys_sem_state(uint64_t regs[]){
    sem_state();
    return;
}
void sys_write(uint64_t  regs[] ){
    char * buffer = (char *) regs[R13]; 
    int size = regs[R15];
    int writeID = getWritePipe();
    
    if(writeID == STDOUT){
        print(buffer, size);
    }
    else{
        pipeWrite(writeID, buffer, size); 
    }
}
 

void sys_read(uint64_t  regs[]){
    char * c = (char *) regs[R13];
    int readID = getReadPipe(), fg = processIsInForeground();
    
    if(fg && readID == STDIN)
        pipeRead(readID, c, 1);
    else if(!fg && readID == STDIN)
        return; //no te toca leer, bloqueate
    else if(!fg && readID != STDIN)
        pipeRead(readID, c, 1);
    else if(fg && readID != STDIN)
        return; //algo salió mal acá 
}

void sys_getReg(uint64_t  regs[]){
    uint64_t * destination = (uint64_t *) regs[R13];
    uint64_t * regs2 =  getRegs();
    for(int i = 0; i < 16; i++){
        destination[i] = regs2[i];
    }
  
}

void sys_time(uint64_t  regs[]){
    int * destination = (int *) regs[R13];  
    int time[3];
    getTime(time);
    for(int i=0; i<3; i++){ //copio la hora!
        destination[i] = time[i];
    }
}

void sys_getMem(uint64_t  regs[]){
    uint8_t * destination = (uint8_t *) regs[R13];
    uint8_t * start = (uint8_t *) regs[R15];
    for(int i = 0; i<32; i++){ //TODO: verificar este cambio a uint8_t
        destination[i] = memContent(start+i);
    }
}

void sys_cpuinfo(uint64_t  regs[]){
    char * rtaVendor = (char *) regs[R13];
    char * rtaBrand = (char *) regs[R15];
    char buffer1[13], buffer2[49];
    cpuVendor(buffer1);
    cpuBrand(buffer2);

    for(int i=0; i < 13; i++){
        rtaVendor[i] = buffer1[i];
    }
    
    for(int i=0; i<49; i++){
        rtaBrand[i] = buffer2[i];
    }

}

void sys_temp(uint64_t  regs[]){
    uint64_t * rta = (uint64_t *) regs[R13];
    rta[0] = cpuTemperature();
}

void sys_context(uint64_t  regs[]){
    int * rta = (int *) regs[R13];
    *rta = getContext();
}

void sys_update_context(uint64_t  regs[]){
    changeContext();
}

void sys_malloc(uint64_t  regs[]){
    int size = (int) regs[R13];
    void ** location = (void **) regs[R15];
    void * res = ltmalloc(size);
    memcpy(location, &res, sizeof(void *));
}

void sys_free(uint64_t  regs[]){
    void * pointer = (void *) regs[R13];
    ltmfree(pointer);
}

void sys_mem(uint64_t  regs[]){
    printMemList();
}

void sys_ps(uint64_t  regs[]){
    processDump();
}

void sys_kill(uint64_t  regs[]){
    int pid = (int) regs[R13];
    processKill(pid);
}

void sys_launch(uint64_t  regs[]){
    void * process = (void *) regs[R13];
    int argc = (int) regs[R15];
    char ** argv = (char**) regs[RBX];
    int * pid_destination = (int *) regs[R10];
    launchProcess(process, argc, argv, pid_destination, regs);
}

void sys_pid(uint64_t  regs[]){
    int * pid = (int *) regs[R13];
    *pid=getPID();
}

void sys_loop(uint64_t  regs[]){
    printGreeting();
}

void sys_exit(uint64_t  regs[]){
    exitProcess();
}

void sys_renounce(uint64_t regs[]){
    renounce();
}

void sys_block(uint64_t regs[]){
    int pid = regs[R13];
    processBlock(pid);
}

void sys_nice(uint64_t regs[]){
   int pid = (int) regs[R13];
   int new_prio = (int) regs[R15];
   processNice(pid, new_prio); 
}
void sys_pipe_write(uint64_t regs[]){
    int id = (int) regs[R13];
    char * address = (char *) regs[R15];
    int bytes = (int) regs[RBX];
    pipeWrite(id, address, bytes);
}

void sys_pipe_read(uint64_t regs[]){
    int id = (int) regs[R13];
    char * address = (char *) regs[R15];
    int bytes = (int) regs[RBX];
    pipeRead(id, address, bytes);
}

void sys_pipe_open(uint64_t regs[]){
    int * id = (int *) regs[R13];
    pipeOpen(id);
}

void sys_pipe_close(uint64_t regs[]){
    int id = (int) regs[R13];
    pipeClose(id);
}

void sys_pipe_states(uint64_t regs[]){
    pipeStates();
}

void sys_change_input(uint64_t regs[]){
    int pipeID = (int) regs[R13];
    int pid = (int) regs[R15];
    change_input(pipeID, pid);
}

void sys_change_output(uint64_t regs[]){
    int pipeID = (int) regs[R13];
    int pid = (int) regs[R15];
    change_output(pipeID, pid);
}
