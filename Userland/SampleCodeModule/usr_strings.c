#include "usr_strings.h"
#define BUFFER_SIZE 1024
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
static char charBuffer[3 * BUFFER_SIZE]; 
static char bufferNum[BUFFER_SIZE] = { '\0' };
static char usr_command[BUFFER_SIZE] = { 0 }; 
extern void codeERROR();


//FIXME: scanf con código repetido
void scanf(char * buffer, int size){
    int  current = 0;
    *charBuffer = 0;
    while( *charBuffer != '\n'){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
            buffer[current++] = *charBuffer;
        }        
    }
	buffer[current]='\0';
	return;
}

void show_scanf(char * buffer, int size){
    int  current = 0, deletes=0;
	*charBuffer = 0;
    while( *charBuffer != '\n' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
            buffer[current++] = *charBuffer;
			if(*charBuffer == '\b'){
				if(current-(deletes+1)>=0){ //para no borrar cosas anteriores
					deletes++;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current]='\0';
	return;
}

void show_processed_scanf(char * buffer, int size){
	int  current = 0;
	*charBuffer = 0;
    while( *charBuffer != '\n' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
			
			if(' ' <= *charBuffer && *charBuffer < 127 ){ //es una letra, número o signo de puntuación, '\b' = 127
				putChar(*charBuffer);
				buffer[current++] = *charBuffer;
			}
			else if(*charBuffer == '\t'){
				for(int i=0; i<5;i++){
					buffer[current++] = ' ';
				}
			}
			else if(*charBuffer == '\b'){
				if(current>0){ //para no borrar cosas anteriores
					current--;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current]='\0';
	return;
}

void scanf_for_calculator(char * buffer, int size){
	int  current = 0;
	*charBuffer = 0;
    while( *charBuffer != '=' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
			
			if(' ' <= *charBuffer && *charBuffer < 127 ){ //es una letra, número o signo de puntuación, '\b' = 127
				putChar(*charBuffer);
				buffer[current++] = *charBuffer;
			}
			else if(*charBuffer == '\t'){
				for(int i=0; i<5;i++){
					buffer[current++] = ' ';
				}
			}
			else if(*charBuffer == '\b'){
				if(current>0){ //para no borrar cosas anteriores
					current--;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current-1]='\0';
	return;
}

void show_numeric_scanf(char * buffer, int size){
	int  current = 0;
	*charBuffer = 0;
	
    while( *charBuffer != '\n' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){	
			if('0' <= *charBuffer && *charBuffer <= '9' ){ //es una letra, número o signo de puntuación, '\b' = 127
				putChar(*charBuffer);
				buffer[current++] = *charBuffer;
			}
			else if(*charBuffer == '\b'){
				if(current>0){ //para no borrar cosas anteriores
					current--;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current]='\0';
	return;
}

void puts(char * string){
	int length = strlen(string);
	put(string, length);
	return;
}

void putChar(char c){
    *charBuffer = c;
    put(charBuffer , 1);
	return;
}

void newline(){
	putChar('\n');
}

int strlen(char * string){
	int count = 0;
	while(string[count++] != 0);
	return count-1;
}

int strequals(char * s1, char * s2){
	int l1 = strlen(s1), l2=strlen(s2);
	int min = (l1<l2)? l1 : l2;
	if ( l1 != l2){
		return 0;
	}
	int equals = 1;
	for(int i=0; i < min && equals; i++){
		if(s1[i] != s2[i]){
			return 0;
		}
	}
	return 1;
}
