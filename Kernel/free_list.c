#ifndef BUDDY

#include "mem_manager.h"
#define FALSE 0
#define TRUE 1
#define MAX_AUX_NODES ((0x100000 / sizeof(Node))+1)
#define MEM_INIT_ADDRESS 0x600000
#define MEM_END_ADDRESS  0xFFFFFFF
#define HEAP_INIT_ADDRESS (MEM_INIT_ADDRESS + sizeof(Node) * MAX_AUX_NODES)
#define TOTAL_MEM_AVAILABLE (MEM_END_ADDRESS - MEM_INIT_ADDRESS)
//guardo el tamaño de mi bloque de memoria libre, su dirección y un pointer al siguiente
typedef struct Node{
    size_t size;
    void * address;
    struct Node * next;
} Node;
typedef struct MemList
{
    Node * first;
    Node * last;
} MemList;

typedef struct FreeList
{
    Node * first;
} FreeList;

static Node * node_vec = (void *) MEM_INIT_ADDRESS; //vector de nodos auxiliares
static int node_index = 0; //lugar disponible en node_vec

static int mem_initialized = FALSE;
static MemList mem_list;
static FreeList free_list;
static int free_initialized = FALSE;

static Node * getAuxNode(){
    if(node_index == MAX_AUX_NODES){
        return NULL;
    }
    return &node_vec[node_index++];
}

//inicializo mi lista
static void init_mem(size_t first_block_size){
    Node * block_to_add = getAuxNode();
    if(block_to_add == NULL){ //no debería pasar nunca
        printS("MAXIMO NUMERO DE NODOS!\n");
        return;
    }

    block_to_add->size=first_block_size;
    block_to_add->address = (void *) HEAP_INIT_ADDRESS; //después del espacio que le asigno a las estructuras auxiliares
    block_to_add->next = NULL;
    
    mem_list.first = block_to_add;
    mem_list.last = mem_list.first;
    mem_initialized = TRUE; 
}

//genero un puntero a una zona de memoria con size lugar disponible
void * ltmalloc(size_t size ){
    if(size <= 0 || size > TOTAL_MEM_AVAILABLE){
        return NULL;
    }
    
    if(free_initialized){
        Node * current = free_list.first;
        Node * previous = current;

        while(current->size < size && current->next != NULL){
            previous = current;
            current = current->next;
        }//si es == NULL llegue al final y no hay ningun bloque que me sirva, sigo de largo
        if(current->size >= size){ //Pongo un límite en lo que me puedo pasar de mambo
            if( current == free_list.first ){ //agarré el primero de la lista
                if(current->next==NULL){ //sólo había un nodo
                    free_list.first = NULL;
                    free_initialized=FALSE; 
                }
                else{//hay otros 
                    free_list.first=current->next;
                }
            }
            previous->next = current->next;
            return current->address;
        }      
        
    }
    //sigo por acá si no encontré un bloque liberado que me sirva

    if(!mem_initialized){
        init_mem(size);
        return mem_list.first->address;
    }


    else{ //lo agrego al final de mi heap(y de la lista)
         
        Node * block_to_add= getAuxNode();
        if(block_to_add == NULL){ 
            printS("MAXIMO NUMERO DE NODOS!\n");
            return NULL;
        }
        block_to_add->size = size;
        void * newAddress = (void *) (mem_list.last->address + mem_list.last->size);
        block_to_add->address = newAddress;
        block_to_add->next = NULL;

        mem_list.last->next = block_to_add;
        mem_list.last = block_to_add; 
        return newAddress; 
    }
    return NULL;
}


void ltmfree(void * pointer){
    Node * previous;
    Node * mem_iterator = mem_list.first;
  
    while(mem_iterator->address < pointer && mem_iterator != NULL){ //busco el nodo que me sirve
        previous = mem_iterator;
        mem_iterator = mem_iterator->next;
    } 
    if(mem_iterator == NULL){//no lo encontré, no era un puntero válido
        return;
    }
    else{ //mem_iterator quedó parado en el nodo que quiero liberar
        
        Node * block_to_add=getAuxNode(); //quiero hacer una copia para insertar en free_list 
        if(block_to_add == NULL){ 
            printS("MAXIMO NUMERO DE NODOS!\n");
            return;
        }
        block_to_add->size = mem_iterator->size;
        block_to_add->address = mem_iterator->address;

        //lo que me queda por setear es el next

        if(!free_initialized){ //no había liberado nada antes           
            free_list.first = block_to_add;
            block_to_add->next = NULL;
            free_initialized = TRUE;
        }
        else{
            Node * free_iterator;
            free_iterator = free_list.first; 
            previous = free_iterator;
            //quiero insertar el it y que free_list quede de menor a mayor segun size
            while(free_iterator->size < block_to_add->size && free_iterator != NULL){ //busco la posicion en free_list
                previous = free_iterator;
                free_iterator = free_iterator->next;
            }
        
            block_to_add->next = free_iterator; // si es el primero, el proximo soy yo?
            if( free_iterator != previous) {
                previous->next = block_to_add;
            }
        }
    }

}

void printMem(){
    if(!mem_initialized){
        printS("No asignaste memoria dinamica!\n");
    }
    else{
        Node * iterator = mem_list.first;
        while(iterator->next != NULL){        
            for(uint8_t i = 0; i < iterator->size; i++){
                printS("0x");
                printHex( (uint64_t) iterator->address+i);
                printS(": ");
                printHex( memContent((uint8_t *)iterator->address+i) );
                newline();
            }
            iterator = iterator->next;
        }
    }
}

#endif