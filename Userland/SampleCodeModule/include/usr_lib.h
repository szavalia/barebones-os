#ifndef _USR_LIB_H_
#define _USR_LIB_H_
   #include <stdint.h>
   #include "usr_strings.h"
   #include "usr_math.h"

   void getTime(int * destination);
   void printTime();

   void getReg(uint64_t * regs);
   void printReg(uint64_t value);
   void inforeg();

   void getMem(uint8_t * dir, uint8_t* bytes);
   void printmem(uint8_t * dir);

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
   void debug(); //FIXME: sacar esto
#endif


