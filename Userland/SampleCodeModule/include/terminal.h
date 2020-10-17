#ifndef _TERMINAL_H_
#define _TERMINAL_H_ 
    #include <stdint.h>
    #include "usr_lib.h"
    #include "usr_strings.h"
    #include "usr_math.h"

    #define NULL (void *) 0 //FIXME: esto no debería estar incluido de algún lado?
    #define BUFFER_SIZE 1024
    #define COMMAND_BUFFER_SIZE 50
    #define MAX_ARGS 5
    #define MAX_COMMANDS 20
    #define TRUE 1
    #define FALSE 0

    
    void printTime(int argc, char ** argv);

    void inforeg(int argc, char ** argv);

    void printmem(int argc, char ** argv);

    void printCPUInfo(int argc, char ** argv);

    void printTemp(int argc, char ** argv);

    void help(int argc, char ** argv);
    void sh(int argc, char ** argv); //lanza la terminal

    void ps(int argc, char ** argv);
    void kill(int argc, char ** argv);
    void cat(int argc, char ** argv);
    void wc(int argc, char ** argv);
    void filter(int argc, char ** argv);
    void block(int argc, char ** argv);
    extern void codeError(int argc, char ** argv);
    void divError(int argc, char ** argv);
#endif