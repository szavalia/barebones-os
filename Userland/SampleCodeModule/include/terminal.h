#ifndef _TERMINAL_H_
#define _TERMINAL_H_ 
    #include <stdint.h>
    #include "usr_lib.h"
    #define NULL (void *) 0 //FIXME: esto no debería estar incluido de algún lado?
    #define BUFFER_SIZE 1024
    #define COMMAND_BUFFER_SIZE 50
    #define MAX_ARGS 5
    #define TRUE 1
    #define FALSE 0

    void getTime(int * destination);
    void printTime();

    void getReg(uint64_t * regs);
    void printReg(uint64_t value);
    void inforeg();

    void getMem(uint8_t * dir, uint8_t* bytes);
    void printmemWrapper();

    void getCPUInfo(char * vendor, char * brand);
    void printCPUInfo();

    void getTemp(uint64_t *temp);
    void printTemp();

    void bootMsg(); //prompt al bootear
    void help();
    void sh(); //lanza la terminal

    void ps();
    void kill();
    void launchProcess();
    
    extern void codeERROR();
    void error();
#endif