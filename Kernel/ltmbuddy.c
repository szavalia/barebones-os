#include <stdio.h>
#include <math.h>
#define INITIAL_POINTER 0x600000
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


blockNode_t * misNodos = FREE_MEMORY;
blockNode_t * parent;
int initialized = 0;
int child_counter;

typedef struct blockNode{
    char flag;
    char divided;
    char level; // 0 = left 1 = right;
    struct blockNode_t *parent;
    struct blockNode_t *left;
    struct blockNode_t *right;
    void * pointer;
}blockNode_t;


void * ltmalloc( size_t size);

void ltmfree( (void *) pointer);

void generateTree(){
    int i;
        misNodos[0].flag= 0;
        misNodos[0].divided = 0;
        misNodos[0].level = MAX_POWER;
        misNodos[0].pointer = INITIAL_POINTER;
        parent = &misNodos[0];
    for( i = 1 ; i < NEEDED_NODES + 1 ; i++){
        misNodos[i].flag = 0;
        misNodos[i].divided = 0;
        misNodos[i].level = 255;
        misNodos[i].pointer = INITIAL_POINTER;
     }
     child_counter = 1;
}

void initialize(){
        generateTree();
        initialized = 1;
}

void * getFreeMem( int neededPower ) {
    int actualPower = parent->level;
    int flag = 0;
    if ( parent->level < neededPower ){
        exit(1);
    }

    if ( parent->level == neededPower){
        if ( flag == 1){
            exit(2);
        }
        else
        {
            return parent->pointer;
        }
        
    }
    blockNode_t actual = parent;
    while ( flag == 0 ){
        if(  ){

        }
    }


}

void * ltmalloc( size_t size ){
    int iterator;
    int neededPower = (int)ceiling(log(size)/log(2));
    if( initialized == 0 ){
        initialize();
    }
    return getFreeMem( neededPower );
}



