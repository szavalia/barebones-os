#ifndef _USR_LIB_H_
#define _USR_LIB_H_
   #include <stdint.h>
   void scanf(char * destination, int length);
   void put(char * source, int length);
   void puts(char * source);
   void scanChar(char * destination);
   void show_scanf(char * buffer, int size);
   void show_processed_scanf(char * buffer, int size);
   void scanf_for_calculator(char * buffer, int size);
   void putChar(char c);
   void newline();

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

   int strcmp(char * s1, char * s2);
   int strlen(char * string);
   void show_scanf(char * buffer, int size);

   void printBase(uint64_t value, uint32_t base);
   void printDec(uint64_t value);
   void printHex(uint64_t value);
   void printBin(uint64_t value);
   void printWithDecimals(double value);
   long parteEntera(uint64_t value);

   extern void getContext(int * context);
   extern void changeContext();
#endif


