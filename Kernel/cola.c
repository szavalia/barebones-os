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
    head->size+=1;
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
    Node * aux = head->first;
    head->size-=1;
    head->first = head->first->next;
    ltmfree(aux);
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
    head->size = 0;
    ltmfree(head);
}

int peek( queueADT head){
    if(head->first == NULL){
        return -1;
    }else{
        return head->first->value;
    }
}

void peekAll( queueADT head ){
    Node * aux = head->first;
    int i;
    for ( i = 0; aux != NULL ; i++){
        printDec((long)aux->value);
        printS(" ");
        aux = aux->next;
    }
    newline();
}