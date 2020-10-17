#include "mem_manager.h"
#include "process.h"
#include "cola.h"

typedef struct Node{
    int value;
    struct Node* next;
}Node;

typedef struct queueCDT{
    int size;
    Node* first;
}queueCDT;


queueADT create_queue(){
    queueADT head = ltmalloc(sizeof(queueCDT));
    head->size = 0;
    head->first = NULL;
    return head;
}

void queue( queueADT head , int value ){
    if ( value < 0 ){
        return;
    }
    Node *newNode = (Node *) ltmalloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    if ( head->first == NULL){
        head->first = newNode;
    }else{
        Node * aux = head->first;
        while ( aux-> next != NULL){
            aux = aux-> next;
        }
        aux->next = newNode;
    }
}


int dequeue(queueADT head){
    if ( head->size <= 0 || head->first == NULL){
        return -1;
    }
    int value = head->first->value;
    head->first = head->first->next;
    return value;
}

int sizeQueue(queueADT head){
    return head->size;
}

void freeQueue(queueADT head){
    Node * aux1 = head->first;
    Node * aux2 = aux1->next;
    while( aux1->next != NULL){
        ltmfree(aux1);
        aux1 = aux2;
        aux2 = aux2->next;
    }
    if( aux2 != NULL){
        ltmfree(aux2);
    }
}