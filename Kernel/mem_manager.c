#include "mem_manager.h"
#include "video_driver.h"
#define FALSE 0
#define TRUE 1
#define MAX_AUX_NODES 32768
#define MEM_INIT_ADDRESS 0x600000
#define MEM_END_ADDRESS  0xFFFFFFF
#define HEAP_INIT_ADDRESS (MEM_INIT_ADDRESS + sizeof(Node) * MAX_AUX_NODES)
#define TOTAL_MEM_AVAILABLE (MEM_END_ADDRESS - MEM_INIT_ADDRESS)
//guardo el tamaño de mi bloque de memoria libre, su dirección y un pointer al siguiente
typedef struct Node{
    size_t size;
    int occupied;
    void * address;
    struct Node * next;
} Node;
typedef struct List
{
    Node * first;
    Node * last;

} List;

static Node * node_vec = (void *) MEM_INIT_ADDRESS; //vector de nodos auxiliares
static int node_index = 0; //lugar disponible en node_vec

static int mem_initialized = FALSE;
static unsigned int mem_available = TOTAL_MEM_AVAILABLE;
static List mem_list;
static List free_list;
static int free_initialized = FALSE;

static Node * getAuxNode(){
    if(node_index == MAX_AUX_NODES){
        return NULL;
    }
    return &node_vec[node_index++];
}

//inicializo mi lista
static void init_mem(size_t first_block_size){
    Node * newFirst = getAuxNode();
    newFirst->size=first_block_size;
    newFirst->address = (void *) HEAP_INIT_ADDRESS; //después del espacio que le asigno a las estructuras auxiliares
    newFirst->occupied = TRUE;
    newFirst->next = NULL;
    
    mem_list.first = newFirst;
    mem_list.last = mem_list.first;
    mem_initialized = TRUE;  
    }

//genero un puntero a una zona de memoria con size lugar disponible
void * ltmalloc(size_t size ){
    /*
    Si la lista está vacía, lo creo a medida y lo marco ocupado
    Si la lista NO está vacía, busco el primer bloque en el que entre y se lo doy, marcándolo ocupado
    --> si no encuentro uno en el que entre, chequeo que tenga lugar y le agrego uno a medida, marcándolo ocupado
        --> Si no hay lugar, devuelvo NULL
    */
    if(size <= 0 || size > mem_available){
        return NULL;
    }
    //TODO: mitosis cuando encuentro un bloque que me sirva, quiero agarrar el tamaño justo y guardar lo que me sobra en free_list
    if(free_initialized){
        Node * previous;
        Node * current = free_list.first;
        while(current->size < size && current->next != NULL){
            previous = current;
            current = current->next;
        }
        if(current->size >= size ){ //si es == NULL llegue al final y no hay ningun bloque que me sirva, sigo de largo
            current->occupied = TRUE;
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
         
        Node * myNode= getAuxNode();
        myNode->size = size;
        void * newAddress = (void *) (mem_list.last->address + mem_list.last->size);
        myNode->address = newAddress;
        myNode->occupied = TRUE;
        myNode->next = NULL;
        mem_list.last->next = myNode;
        mem_list.last = myNode; //acá estaba &myNode
        return newAddress; 
    }
    return NULL;
}


void ltmfree(void * pointer){
    Node * previous;
    Node * mem_iterator = mem_list.first;

    while(mem_iterator->address != pointer && mem_iterator != NULL){ //busco el nodo que me sirve
        previous = mem_iterator;
        mem_iterator = mem_iterator->next;
    }
    if(mem_iterator == NULL){
        return;
    }
    else{ //mem_iterator quedó parado en el nodo que quiero liberar
        mem_iterator->occupied = FALSE;
        
        Node * block_to_add=getAuxNode(); //quiero hacer una copia para insertar en free_list 
        block_to_add->size = mem_iterator->size;
        block_to_add->occupied = FALSE;
        block_to_add->address = mem_iterator->address;
        //lo que me queda por setear es el next
        if(!free_initialized){
            free_list.first = block_to_add;
            block_to_add->next = NULL;
            free_list.last = free_list.first;
            free_initialized = TRUE;
        }
        else{
            Node * free_iterator;
            free_iterator = free_list.first; 
            //para insertar el it y que free_list quede de menor a mayor
            while(free_iterator->size < block_to_add->size && free_iterator->next != NULL){ //busco la posicion en free_list
                previous = free_iterator;
                free_iterator = free_iterator->next;
            }
            if(free_iterator->next == NULL){
                free_iterator->next = block_to_add;
                block_to_add->next = NULL;
                free_list.last = block_to_add;
            }
            else{
                block_to_add->next = free_iterator;
                previous->next = block_to_add;
            }
        }
    }
}