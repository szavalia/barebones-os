#ifndef _USR_LIB_H_
#define _USR_LIB_H_
   #include <stdint.h>
   #include "usr_strings.h"
   #include "usr_math.h"
   #include "usr_sem.h"

   #define NULL (void *) 0 
   #define TRUE 1
   #define FALSE 0   


   
   //TP Arqui
   extern void getContext(int * context);
   extern void changeContext();
   extern void getMem(uint8_t * dir, uint8_t* bytes);
   extern void getTime(int * destination);
   extern void getCPUInfo(char * vendor, char * brand);
   extern void getReg(uint64_t * regs);
   extern void getTemp(uint64_t *temp);
   

   //Processes and scheduling
   #define MAX_PROCESSES 5
   void loop(int argc, char **argv);
   void exit(int argc, char **argv);

   extern void callPs();
   extern void callKill(int pid);
   extern void callLaunch( void * process , int argc , char * argv[], int * pid_destination );
   extern void callLoop();
   extern void callExit();
   extern void callNice(int pid, int priority);
   extern void getPID(int * destination); 
   extern void renounceCPU();
   extern void blockProcess(int pid);


   //Memory management
   void * ltmalloc(int size);
   void ltmfree(void * pointer);

   extern void callMalloc(int size, void ** location);
   extern void callFree(void * pointer);
   extern void mem();

   //Pipes
   #define PIPESIZE 1024
   void pipe(int argc, char **argv);

   extern void pipeWrite(int pipeID, char * address, int bytes);
   extern void pipeRead(int pipeID, char * address, int bytes);
   extern void pipeOpen(int * destinationForID);
   extern void pipeClose(int pipeID);
   extern void callPipe();

   extern void change_input(int id, int pid);

#endif


