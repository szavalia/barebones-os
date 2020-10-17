// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "user_interrupts.h"
#include "video_driver.h"
#include "keyboard.h"
#include "time.h"
#include "mem_manager.h"
#include "process.h"
#include "lib.h"
#include "reg_t.h"

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

    }
    return 1;
}

void sys_sem_init(){
    return;
}

void sys_sem_wait(){
    return;
}
void sys_sem_post(){
    return;
}
void sys_write(uint64_t  regs[] ){
    char * buffer = (char *) regs[R13]; 
    int size = getR15();
    print(buffer, size);
}
 

 void sys_read(uint64_t  regs[]){
    char * c = (char *) regs[R13];
    if(processIsInForeground()){
        *c = readChar(); //si no hay nada en el buffer, te retorna un 0    
    }
}

void sys_getReg(uint64_t  regs[]){
    uint64_t * destination = (uint64_t *) getR13();
    uint64_t * regs2 =  getRegs();
    for(int i = 0; i < 16; i++){
        destination[i] = regs2[i];
    }
  



}

void sys_time(uint64_t  regs[]){
    int * destination = (int *) getR13();  
    int time[3];
    getTime(time);
    for(int i=0; i<3; i++){ //copio la hora!
        destination[i] = time[i];
    }
}

void sys_getMem(uint64_t  regs[]){
    uint8_t * destination = (uint8_t *) getR13();
    uint8_t * start = (uint8_t *) getR15();
    for(int i = 0; i<32; i++){ //TODO: verificar este cambio a uint8_t
        destination[i] = memContent(start+i);
    }
}

void sys_cpuinfo(uint64_t  regs[]){
    char * rtaVendor = (char *) getR13();
    char * rtaBrand = (char *) getR15();
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
    uint64_t * rta = (uint64_t *) getR13();
    rta[0] = cpuTemperature();
}

void sys_context(uint64_t  regs[]){
    int * rta = (int *) getR13();
    *rta = getContext();
}

void sys_update_context(uint64_t  regs[]){
    changeContext();
}

void sys_malloc(uint64_t  regs[]){
    int size = (int) getR13();
    void ** location = (void **) getR15();
    void * res = ltmalloc(size);
    memcpy(location, &res, sizeof(void *));
}

void sys_free(uint64_t  regs[]){
    void * pointer = (void *) getR13();
    ltmfree(pointer);
}

void sys_mem(uint64_t  regs[]){
    printMemList();
}

void sys_ps(uint64_t  regs[]){
    processDump();
}

void sys_kill(uint64_t  regs[]){
    int pid = (int) getR13();
    processKill(pid);
}

void sys_launch(uint64_t stack_pointer){ //FIXME: en el switch le paso un uint64_t *
    void * process = (void *) getR13();
    int argc = (int) getR15();
    char ** argv = (char**) getRBX();
    launchProcess(process, argc, argv, stack_pointer);
}

void sys_pid(uint64_t regs[]){
    int * pid = getR13();
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
    int pid = getR13();
    blockProcess(pid);
}