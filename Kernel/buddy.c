#ifdef BUDDY

#include "mem_manager.h"
#define FALSE 0
#define TRUE 1

#define MIN_BLOCK_POWER 10 // 1k           
#define MAX_BLOCK_POWER 27 // 128MB
#define MAX_LEVEL_COUNT ( MAX_BLOCK_POWER - MIN_BLOCK_POWER + 1 )  // cantidad de niveles
#define MAX_LEVEL ( MAX_BLOCK_POWER - MIN_BLOCK_POWER ) // INDEX MAX DE NIVELES
#define MAX_AUX_BLOCKS ((0x100000 / sizeof(Block))+1)
#define MEM_INIT_ADDRESS (void *) 0x600000
#define MEM_END_ADDRESS  (void *) 0xFFFFFF
#define HEAP_INIT_ADDRESS (void *) (MEM_INIT_ADDRESS + sizeof(Block) * MAX_AUX_BLOCKS)
#define TOTAL_MEM_AVAILABLE (uint64_t) (MEM_END_ADDRESS - MEM_INIT_ADDRESS)
#define ALIVE 1

typedef struct Block{
    int occupied; 
    void * lowerBound;
}Block;
typedef struct Level{
    int cant_blocks;
    Block * array;
}Level;


static Level levels[MAX_LEVEL_COUNT]; // Hasta 1GB de blocks 
static int mem_initialized = 0;
void initialize();
int pow2(int exp);
void create_nodes();
int get_level( int size );
int get_buddy( int index );
int im_left( int index );
int left_child(int index);
int right_child(int index);
int parent(int index);
int is_power_of_2(int index);                                    
int log2( int num );
int check_space_recursive(int level, int index);
int get_index_by_level( uint64_t address , int level  );
void initialize(){
    create_nodes();
    mem_initialized = 1;
}

int pow2(int exp){
    return (1 << exp);
}

void create_nodes(){
    int i = 0;
    int max_level = MAX_LEVEL;
    int size_of_block = sizeof(Block);
    int block_count;
    void * block_start = MEM_INIT_ADDRESS;
    //doy espacio para los blockes
    while ( max_level - i >= 0  ){ //itero por todos los niveles, del menos granulado al más granulado
        block_count = pow2(i); //el numero de bloques en mi nivel
        levels[max_level - i].cant_blocks = block_count; 
        levels[max_level - i].array= block_start;
        block_start+= size_of_block*(block_count); 
        i++;
    }
    levels[max_level].array[0].lowerBound= HEAP_INIT_ADDRESS;
    levels[max_level].array[0].occupied = FALSE;
}

int get_index_by_level( uint64_t address , int level  ){
    return address / pow2(level + MIN_BLOCK_POWER);
}

int get_level ( int size ){
    int level = log2(size) - MIN_BLOCK_POWER;
    return ( level < 0 )? 0 : level;
} 

int get_buddy( int index ){
    return ( index % 2 == 0)? index + 1 : index -1;
}

int im_left( int index ){
    return index % 2 == 0;
}

int left_child(int index){ 
    return (index << 1); 
}

int right_child(int index){
    return ((index << 1) + 1);
}

int parent(int index){
    return (index>>1);
}            
 int is_power_of_2(int index){                                    
   return (index != 0) && ((index & (index - 1)) == 0);
 } 


int log2( int num ){
    int i = 0;
    int aux = num;
    if(num != 0){
        while ( aux > 0 ){
            i++;
            aux = aux >> 1;
        }
    }
    if ( !is_power_of_2(num)){
        i++;
    }
    return i-1;
}

int check_space_recursive(int level, int index){
    if(level == MAX_LEVEL){ //caso base: no tengo buddy
        if(levels[level].array[index].occupied){
            return FALSE; 
        }
        else{
            levels[level].array[index].occupied = TRUE;
            return TRUE;
        }
    }

    if(levels[level].array[get_buddy(index)].occupied){ //chequeo si mi buddy está ocupado
        levels[level].array[index].occupied = TRUE;
        return TRUE;
    }
    else if(levels[level+1].array[parent(index)].occupied){ //mi buddy no está ocupado, pero mi padre sí
        return FALSE; //reservaron todo mi padre, no puedo usar este espacio
    }
    else{
        if(check_space_recursive(level+1, parent(index))){
            levels[level].array[index].occupied = TRUE;
            return TRUE;
        }
        else{
            return FALSE;
        }
    }
}

//chequea si el lugar me sirve, dejando listas las estructuras internas(int level, int index, int )
int check_space(int level, int index){ 
    return check_space_recursive(level, index);
}

void * ltmalloc(size_t size ){
    if(!mem_initialized)
        initialize();
    
    if(size <= 0 || size > TOTAL_MEM_AVAILABLE){
        return NULL;
    }
    
    int level = get_level(size);
    for(int i=0; i < levels[level].cant_blocks; i++){ //busco un bloque disponible en su nivel
        if(!levels[level].array[i].occupied && check_space(level , i)){
            return HEAP_INIT_ADDRESS + i * pow2(level + MIN_BLOCK_POWER);
        } 
    }
    return NULL;    
}        

void ltmfree(void * pointer){
    uint64_t address = pointer - HEAP_INIT_ADDRESS;

    if(!mem_initialized)
        initialize();

    if( address < 0 || address > pow2(MAX_BLOCK_POWER) || address % pow2(MIN_BLOCK_POWER) != 0 ){ //no es un bloque válido
        return;
    }    

    int index;
    for( int level = 0 ; level < MAX_LEVEL_COUNT ; level++){ //navego por mis niveles
        if ( address % pow2(level + MIN_BLOCK_POWER) != 0 ){ //hasta que no esté alineado
            return; //ya no voy a encontrar mi bloque       
        }
        index = get_index_by_level(address, level);

        if(level == MAX_LEVEL){ //estoy en el nivel maximo: no hay buddy
            levels[level].array[index].occupied = FALSE;
            return;
        }

        if(!levels[level].array[index].occupied){
            if(levels[level].array[get_buddy(index)].occupied){
                return; //el ocupado es el buddy!
            }
        }
        else{
            levels[level].array[index].occupied = FALSE;
            if(levels[level].array[get_buddy(index)].occupied){
                return;
            }           
        }        
    }
}

void printMem(){
    uint64_t * mem = HEAP_INIT_ADDRESS;
    long i;
    for ( i = 0 ; i < (pow2(MAX_BLOCK_POWER)/8) ; i++){
        printHex(&(mem[i]));
        printS(": ");
        printHex((uint64_t) mem[i]);
        newline();
    }
}


#endif



