#include "mem_manager.h"

#ifndef __queueADT_h_
#define __queueADT_h_

typedef struct queueCDT * queueADT;

queueADT create_queue();

void queue( queueADT head , int value );
int dequeue(queueADT head);
int sizeQueue(queueADT head);
void freeQueue(queueADT head);
int peek( queueADT head);
void peekAll( queueADT head);
#endif