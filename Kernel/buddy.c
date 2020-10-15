#include "mem_manager.h"
#include "video_driver.h"
#define FALSE 0
#define TRUE 1

typedef struct Pair{
    int flag; 
    void * lowerBound;
    void * UpperBound;
}Pair;
#define MAX_AUX_NODES ((0x100000 / sizeof(Pair))+1)
#define MEM_INIT_ADDRESS 0x600000
#define MEM_END_ADDRESS  0xFFFFFF
#define HEAP_INIT_ADDRESS (MEM_INIT_ADDRESS + sizeof(Pair) * MAX_AUX_NODES)
#define TOTAL_MEM_AVAILABLE (MEM_END_ADDRESS - MEM_INIT_ADDRESS)
/*128MB=2^27
4K=2^12
128MB/4K=2^15 --> un "arbol" de 16 niveles; el 2^0 cuenta


*/
#define ALIVE 1

typedef struct Pair{
    int flag; 
    void * lowerBound;
    void * upperBound;
}Pair;

typedef struct Level{
    int max_size;
    int actual_size;
    Pair * array;
}Level;


Level levels[17]; // Hasta 1GB de blocks 

void initialize(){
    int size =


}


void createNodes(){
    int i = 0;
    int max_power = 15;
    int size = sizeof(Pair);
    int power;
    int acum = 1;
    void * pair_start = MEM_INIT_ADDRESS;
    int block_space = HEAP_INIT_ADDRESS - MEM_INIT_ADDRESS; 
    //doy espacio para los blockes
    while ( max_power - i >= 0 ){
        power = pow(2,i);
        levels[max_power - i].max_size = power;
        levels[max_power - i].actual_size = 0;
        levels[max_power - i].array= pair_start;
        pair_start+= size*(power + 1 ); //+1 para simplificar calculos
        acum += power; // cuantos bloques llevo
        i++;
    }
    levels[max_power].actual_size=1; // 1 significa broken comleto
    levels[max_power].array[0].lowerBound= HEAP_INIT_ADDRESS;
    levels[max_power].array[0].upperBound= HEAP_INIT_ADDRESS + (acum)* 4 * 1024;
    levels[max_power].array[0].flag = 0;
}

void * ltmalloc(size_t size ){
    int level = (log10(size)/log10(2)) - 12;

}

inline int left_child(int index){
    /* index * 2 + 1 */
    return ((index << 1) + 1); 
}

inline int right_child(int index){
    /* index * 2 + 2 */
    return ((index << 1) + 2);
}

 inline int parent(int index){
    /* (index+1)/2 - 1 */
    return (((index+1)>>1) - 1);
}
 int is_power_of_2(int index){
   if( !(index & (index - 1)) ){
       return 1; //true
   }else{ 
       return 0; //false
   }
}








