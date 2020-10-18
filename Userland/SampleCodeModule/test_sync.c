#include "test_sync.h"

#define NULL 0

void * sem_pointer;
int64_t global;  //shared memory
uint64_t sem;
int64_t value;
uint64_t N;
char name_inc1[] = "incSUM";
char name_inc2[] = "incDEC";

void inc1();
void inc2();

uint64_t my_create_process(void *name, int value){
  if ( value == 1){
    char * argv[2] = { name , NULL };
    callLaunch(inc1 , 1 , argv );
  }
  if ( value == 2){
    char * argv[2] = { name , NULL };
    callLaunch(inc2 , 1 , argv );
  }
  return 0;
}

uint64_t my_sem_open(void **sem_id, uint64_t initialValue){
  sem_init(sem_id , initialValue );
  return 1;
}

uint64_t my_sem_wait(void *sem_id){
  sem_wait(sem_id);
  return 0;
}

uint64_t my_sem_post(void *sem_id){
  sem_post(sem_id);
  return 0;
}

uint64_t my_sem_close(void *sem_id){
  sem_wait(sem_id);
  return 0;
}

#define TOTAL_PAIR_PROCESSES 5
#define SEM_ID sem_pointer

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  renounceCPU();
  *p = aux;
}

void inc1(){
  int value = 2;
  uint64_t i;

  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(SEM_ID);
    renounceCPU();
    slowInc(&global, value);
    if (sem) my_sem_post(SEM_ID);
  }
  
  puts("Final value: ");
  printDec(global);
  newline();
  puts("Adios SUM\n");
  callExit();
}


void inc2(){
  int value = -1;
  uint64_t i;  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(SEM_ID);
    slowInc(&global, value);
    if (sem) my_sem_post(SEM_ID);
    renounceCPU();
  }

  
  puts("Final value: ");
  printDec(global);
  newline();
  puts("Adios DEC\n");
  callExit();
}

void test_sync(){
  uint64_t i;
  sem = 1;
  N=500;
  global = 0;
  void * ptr;
  sem_pointer= 0;
  my_sem_open(&ptr , 1);
  puts("CREATING PROCESSES...(WITH SEM)\n");
  printDec(sem_pointer);
  newline();
  my_sem_open(&sem_pointer , 1);
  printDec(sem_pointer);
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process(name_inc1, 1);
    my_create_process(name_inc2, 2);
  }
}

void test_no_sync(){
  uint64_t i;
  global = 0;
  sem = 0;
  N=1000;
  void * ptr;
  sem_pointer= 0;
  my_sem_open(&ptr , 4);
  puts("CREATING PROCESSES...(WITHOUT SEM)\n");
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process(name_inc1 , 1);
    puts("Segundo proc\n");
    my_create_process(name_inc2, 2);
  }
  puts("sali del no_\n");
}
