#ifndef _USR_LIB_H_
#define _USR_LIB_H_
   #include <stdint.h>
   #include "usr_strings.h"
   #include "usr_math.h"
   #define NULL (void *) 0 //FIXME: esto no debería estar incluido de algún lado?
   #define BUFFER_SIZE 1024
   #define COMMAND_BUFFER_SIZE 20


   void getTime(int * destination);
   void printTime();

   void getReg(uint64_t * regs);
   void printReg(uint64_t value);
   void inforeg();

   void getMem(uint8_t * dir, uint8_t* bytes);
   void printmem(char * hexDir);

   void getCPUInfo(char * vendor, char * brand);
   void printCPUInfo();

   void getTemp(uint64_t *temp);
   void printTemp();

   void bootMsg(); //prompt al bootear
   void help();
   void launch_terminal();

   extern void getContext(int * context);
   extern void changeContext();

   void * ltmalloc(int size);
   void ltmfree(void * pointer);
   extern void mem();

   extern void codeERROR();
   void initializeCommandBuffer();
#endif


