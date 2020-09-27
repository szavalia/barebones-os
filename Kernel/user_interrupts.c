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


int int80_handler( uint64_t stack_pointer){
    int option = getR12();
    switch(option){
        case 0:
            sys_read();
            break;
        case 1:
            sys_write();
            break;
        case 2:
            sys_getReg();
            break;
        case 3:
            sys_time();
            break;
        case 4:
            sys_getMem();
            break;
        case 5:
            sys_cpuinfo();
            break;
        case 6:
            sys_temp();
            break;
        case 7:
            sys_context();
            break;
        case 8:
            sys_update_context();
            break;
        case 9:
            sys_malloc();
            break;
        case 10:
            sys_free();
            break;
        case 11:
            sys_mem();
            break;
        case 12:
            sys_ps();
            break;
        case 13:
            sys_kill();
            break;
        case 14:
            sys_launch(stack_pointer);
            break;
        case 15:
            return sys_fork(stack_pointer);
            break;
    }
    return 1;
}

void sys_write(){
    char * buffer = (char *) getR13();
    int size = getR15();
    print(buffer, size);
}
 

 void sys_read(){
    char * c = (char *) getR13();
    *c = readChar(); //si no hay nada en el buffer, te retorna un 0    
}

void sys_getReg(){
    uint64_t * destination = (uint64_t *) getR13();
    uint64_t * regs =  getRegs();
    for(int i = 0; i < 16; i++){
        destination[i] = regs[i];
    }
}

void sys_time(){
    int * destination = (int *) getR13();  
    int time[3];
    getTime(time);
    for(int i=0; i<3; i++){ //copio la hora!
        destination[i] = time[i];
    }
}

void sys_getMem(){
    uint8_t * destination = (uint8_t *) getR13();
    uint8_t * start = (uint8_t *) getR15();
    for(int i = 0; i<32; i++){ //TODO: verificar este cambio a uint8_t
        destination[i] = memContent(start+i);
    }
}

void sys_cpuinfo(){
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

void sys_temp(){
    uint64_t * rta = (uint64_t *) getR13();
    rta[0] = cpuTemperature();
}

void sys_context(){
    int * rta = (int *) getR13();
    *rta = getContext();
}

void sys_update_context(){
    changeContext();
}

void sys_malloc(){
    int size = (int) getR13();
    void ** location = (void **) getR15();
    void * res = ltmalloc(size);
    memcpy(location, &res, sizeof(void *));
}

void sys_free(){
    void * pointer = (void *) getR13();
    ltmfree(pointer);
}

void sys_mem(){
    printMemList();
}

void sys_ps(){
    processDump();
}

void sys_kill(){
    int pid = (int) getR13();
    processKill(pid);
}

void sys_launch(uint64_t stack_pointer){
    void * process = (void *) getR13();
    int argc = (int) getR15();
    char ** argv = (char**) getRBX();
    launchProcess(process, argc, argv, stack_pointer);
}

int sys_fork(uint64_t stack_pointer){
    return fork(stack_pointer);
}