#include "mem_manager.h"
#include "video_driver.h"
#define FALSE 0
#define TRUE 1
#define MAX_AUX_NODES ((0x100000 / sizeof(Node))+1)
#define MEM_INIT_ADDRESS 0x600000
#define MEM_END_ADDRESS  0xFFFFFFF
#define HEAP_INIT_ADDRESS (MEM_INIT_ADDRESS + sizeof(Node) * MAX_AUX_NODES)
#define TOTAL_MEM_AVAILABLE (MEM_END_ADDRESS - MEM_INIT_ADDRESS)

#define ALIVE 1
#define BROKEN 0

typedef struct Pair{
    int flag; 
    void * lowerBound;
    void * UpperBound;
}Pair;

typedef struct Level{
    int flag;
    Pair * array;
}Level;


Level levels[]; 

void initialize(){
    int size = 


}