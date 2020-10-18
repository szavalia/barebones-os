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
uint64_t getRAX();
uint64_t getRBX();
uint64_t getRCX();
uint64_t getRDX();
uint64_t getRSI();
uint64_t getRDI();
uint64_t getRBP();
uint64_t getRSP();
uint64_t getR8();
uint64_t getR9();
uint64_t getR10();
uint64_t getR11();
uint64_t getR12();
uint64_t getR13();
uint64_t getR14();
uint64_t getR15();
#endif