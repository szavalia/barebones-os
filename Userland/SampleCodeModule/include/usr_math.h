#ifndef _USR_MATH_H_
#define _USR_MATH_H_
    #include <stdint.h>
    #include "usr_strings.h"
    #include "usr_lib.h"
    #define NUM_BUFFER_SIZE 20

    int hexadecimalToDecimal(char hexVal[]); 
    void printBase(uint64_t value, uint32_t base);
    void printDec(uint64_t value);
    void printHex(uint64_t value);
    void printBin(uint64_t value);
    void printReg(uint64_t value);
    void printWithDecimals(double value);
    long parteEntera(uint64_t value);
#endif