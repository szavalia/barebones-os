#ifndef _USR_LIB_H_
#define _USR_LIB_H_
   #include <stdint.h>
   #include "usr_strings.h"
   #include "usr_math.h"

   #define NULL (void *) 0 //FIXME: esto no debería estar incluido de algún lado?
   #define BUFFER_SIZE 1024
   #define TRUE 1
   #define FALSE 0   
   #define MAX_PROCESSES 5

   

   extern void getContext(int * context);
   extern void changeContext();

   void * ltmalloc(int size);
   void ltmfree(void * pointer);

   
   void loop(int argc, char **argv);
   void exit(int argc, char **argv);
   void pid(int argc, char **argv);
   
   extern void mem();
   extern void getMem(uint8_t * dir, uint8_t* bytes);
   extern void getTime(int * destination);
   extern void getCPUInfo(char * vendor, char * brand);
   extern void getReg(uint64_t * regs);
   extern void callMalloc(int size, void ** location);
   extern void callFree(void * pointer);
   extern void callPs();
   extern void callKill(int pid);
   extern void callLaunch( void * process , int argc , char * argv[] );
   extern void callLoop();
   extern void callExit();
   extern void getTemp(uint64_t *temp);
   extern void getPID(int * destination); 
   extern void renounceCPU();
   extern void blockProcess(int pid);

#endif


