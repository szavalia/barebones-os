#ifndef _USR_LIB_H_
#define _USR_LIB_H_
   #include <stdint.h>
   #include "usr_strings.h"
   #include "usr_math.h"

   #define NULL (void *) 0 //FIXME: esto no debería estar incluido de algún lado?
   #define BUFFER_SIZE 1024
   #define TRUE 1
   #define FALSE 0   

   extern void getContext(int * context);
   extern void changeContext();

   void * ltmalloc(int size);
   void ltmfree(void * pointer);
   extern void mem();

   
   void loop();
   void exit();   

   extern void callMalloc(int size, void ** location);
   extern void callFree(void * pointer);
   extern void callPs();
   extern void callKill(int pid);
   extern void callLaunch( void * process , int argc , char * argv[] );
   extern void callLoop();
   extern void callExit();
   extern void callSemInit( void ** sem , int value);
   extern void callSemWait( void * sem);
   extern void callSemPost( void * sem);
   extern int fork(); //FIXME: deprecated

#endif


