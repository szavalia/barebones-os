#include "mem_manager.h"
#define FALSE 0
#define TRUE 1
#define MEM_INIT_ADDRESS 0x600000
#define MEM_END_ADDRESS 0x9FFFFFFF
#define TOTAL_MEM_AVAILABLE (MEM_END_ADDRESS - MEM_INIT_ADDRESS)
//guardo el tamaño de mi bloque de memoria libre, su dirección y un pointer al siguiente
typedef struct Node{
    size_t size;
    int occupied;
    void * address;
    Node * next;
} Node;
//la lista va a ser ordenada según el tamaño del nodo
typedef struct List
{
    Node * first;
    Node * last;

} List;

static int mem_initialized = FALSE;
static unsigned int mem_available = TOTAL_MEM_AVAILABLE;
static List mem_list;
static List free_list;
static free_initialized = FALSE;

//inicializo mi lista
static void init_mem(size_t first_block_size){
    Node * newFirst;
    newFirst->size=first_block_size;
    newFirst->address = (void *) MEM_INIT_ADDRESS;
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
    if(size <= 0 || size >mem_available){
        return NULL;
    }

    if(free_initialized){
        Node * previous;
        Node * actual = free_list.first;
        while(actual.size < size && actual.next != NULL){
            previous = actual;
            actual = actual.next
        }
        if(actual.next != NULL){ //si es == NULL llegue al final y no hay ningun bloque que me sirva, sigo de largo
            actual->occupied = TRUE;
            void * ret = actual->address;
            previous.next = actual.next;
            return ret;
        }
    }

    if(!mem_initialized){
        init_mem(size);
        return mem_list.first->address;
    }


    else{ //lo agrego al final de mi heap(y de la lista)
        Node * myNode;
        myNode->size = size;
        int newAddress = mem_list.last.address + mem_list.last.size;
        myNode->address = newAddress;
        myNode->occupied = TRUE;
        myNode->next = NULL;
        mem_list.last.next = myNode;
        mem_list.last = &myNode;
        return newAddress; 
    }
    return NULL;
}

void ltmfree1(void * pointer){
    Node * iterator = mem_list->start;
    while(iterator.address != pointer){
        iterator = iterator.next;
    }
    if(iterator.next == NULL && iterator.address != pointer){
        return null;
    }
    else{
        iterator.occupied = FALSE;
    }
}

void ltmfree2(void * pointer){
    Node * previous;
    Node * iterator = mem_list->start;

    while(iterator.address != pointer){ //busco el nodo que me sirve
        previous = iterator;
        iterator = iterator.next;
    }
    if(iterator.next == NULL && iterator.address != pointer){
        return null;
    }
    else{
        previous.next = iterator.next; //saco el pointer de la lista de memoria, solo quedan los ocupados
        iterator.occupied = FALSE;
        if(!free_initialized){
            free_list.first = iterator;
            free_list.last = free_list.first;
            free_initialized = TRUE;
        }
        else{
            Node * actual;
            actual = free_list.first; 
            //para insertar el it y que free_list quede de menor a mayor
            while(actual.size < iterator.size && actual.next != NULL){ //busco la posicion en free_list
                previous = actual;
                actual = actual.next;
            }
            if(actual.next == NULL){
                actual.next = iterator;
                iterator.next = null;
                free_list = NULL;
            }
            else{
                iterator.next = actual;
                previous.next = iterator;
            }
        }
    }
}


//crea un nuevo bloque de memoria en luego de un nodo prev y lo declaro ocupado
static void create_new_node(size_t size, Node * prev){
    //creo mi nuevo nodo
    Node * new;
    new->size = size;
    new->occupied = TRUE;
    new->address = prev->address + prev->size;
    new->next=prev->next;
    
    prev->next = new;

    //caso prev = last
    if(prev = mem_list.last){
        mem_list.last = new;
    }
}

void ltmfree(void * pointer){

}
