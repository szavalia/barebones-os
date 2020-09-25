#include <stdio.h>
#include <math.h>
#define INITIAL_POINTER 0x600 000
#define MAX_POINTER 0xDFFFFF
#define BUDDY_MAXPOINTER 0x7FFFFF + 0x600000
#define FREE_MEMORY BUDDY_MAXPOINTER + 0x1000
#define MIN_SIZE 4096 //4k
#define MIN_POWER 12 //log2(4096)
#define MAX_POWER 23 // floor(log2(0xFFFFFF-0x600000) )
#define NEEDED_NODES 4095 //

#define LEFT_NODE 1   // 0001 1  0010 2  0100 4 1000 8
#define RIGHT_NODE 2 
#define NOT_DIVIDED_NODE 4
#define DIVIDED_NODE 8


char * bitMap;
int initialized = 0;

struct typedef{
    char bit;
    void * start;
    void * finish;
    void * size;
    void *   
}bitBlock_t;

void * ltmalloc( size_t size);

void ltmfree( (void *) pointer);



void initialize(){
      bitMap= (char*) FREE_MEMORY;
      int i;
      for( i = 0 ; i< NEEDED_NODES ; i++){
        bitMap[i] = 0;
      }  

}



void * ltmalloc( size_t size ){
    int neededPower = (int)log(size)/log(2) 
    if ( initialized = 0 ){
        initialize();
    }

}