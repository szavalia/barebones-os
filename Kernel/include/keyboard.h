#include<stdint.h>
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
extern unsigned int getKeyboardScancode(); // viene de keyboard.asm
void keyboard_handler();
char readChar();
void freebuffer(int beginning);
int get_buffer_size();
void saveRegs();
uint64_t * getRegs();

//Get registros
long getRAX();
long getRBX();
long getRCX();
long getRDX();
long getRSI();
long getRDI();
long getRBP();
long getRSP();
long getR8();
long getR9();
long getR10();
long getR11();
long getR12();
long getR13();
long getR14();
long getR15();
#endif