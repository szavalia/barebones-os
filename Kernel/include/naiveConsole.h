#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

//-----------------------------
enum COLOUR {BLACK=0,BLUE=1,GREEN=10,CYAN=11,RED=4,MAGENTA=13,BROWN=6,YELLOW=14,WHITE=15};
void setBackgroundColour(char colour);
void setFontColour(char colour);
void printCharColour(char c);
void printColour(char* s);
void toStartOfLine();
void ncBackspace();
//-----------------------------

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();


#endif