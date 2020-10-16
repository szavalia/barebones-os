#ifndef _USR_STRINGS_H_
#define _USR_STRINGS_H_
    #include <stdint.h>
    void scanf(char * destination, int length);
    void put(char * source, int length);
    void puts(char * source);
    void putChar(char c);
    void scanChar(char * destination);
    void show_scanf(char * buffer, int size);
    void show_processed_scanf(char * buffer, int size);
    void show_numeric_scanf(char * buffer, int size);
    void scanf_for_calculator(char * buffer, int size);
    void newline();
    int strlen(char * string);
    int strequals(char * s1, char * s2);
    char * strtok( char * string , char key );
#endif
