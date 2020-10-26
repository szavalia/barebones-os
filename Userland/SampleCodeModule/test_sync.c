#include "test_sync.h"

#define INC_VALUE 1
#define DEC_VALUE -1
#define OP_TIMES 10000
#define TOTAL_PAIR_PROCESSES 5
#define SEM_ID sem_pointer

void * sem_pointer , *finishing_sem;
int64_t global;  //shared memory
uint64_t sem;
int64_t value;
uint64_t N;
char name_inc[] = "semSUM";
char name_dec[] = "semDEC";

void inc();
void dec();

uint64_t my_create_process(void *name, int value){
  if ( value == 1){
    char * argv[2] = { name , NULL };
    callLaunch(inc , 1 , argv, NULL );
  }
  if ( value == 2){
    char * argv[2] = { name , NULL };
    callLaunch(dec , 1 , argv, NULL );
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
  sem_close(sem_id);
  return 0;
}


void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  renounceCPU();
  *p = aux;
}

void inc(){
  int value = INC_VALUE;
  uint64_t i;

  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(SEM_ID);
    renounceCPU();
    slowInc(&global, value);
    if (sem) my_sem_post(SEM_ID);
  }
  
  puts("Final value: ");
  printDec((uint64_t)global);
  newline();
  puts("Adios SUM\n");
  
  sem_post(finishing_sem);
  callExit();
}


void dec(){
  int value = DEC_VALUE;
  uint64_t i;  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(SEM_ID);
    slowInc(&global, value);
    if (sem) my_sem_post(SEM_ID);
    renounceCPU();
  }

  
  puts("Final value: ");
  printDec((uint64_t)global);
  newline();
  puts("Adios DEC\n");
  sem_post(finishing_sem);
  callExit();
}

void test_setup( int flag){
  
  N=OP_TIMES; // cantidad de operaciones x hijo
  global = 0; //start value
  sem_pointer= 0;
  finishing_sem=0;
  my_sem_open(&finishing_sem , 0);
  printHex((uint64_t)finishing_sem);
  newline();
  if ( flag == 1){
  sem = 1; //flag de sem
  my_sem_open(&sem_pointer , 1);
  printHex((uint64_t)sem_pointer); 
  newline();
  puts("CREATING PROCESSES...(WITH SEM)\n");
  }else{
    sem = 0;
  puts("CREATING PROCESSES...(WITHOUT SEM)\n");
  }
  
}


void launch_processes(){
  for(int i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process(name_inc, 1);
    my_create_process(name_dec, 2);
  }
}

void finishing(){
   for( int i = 0 ; i < TOTAL_PAIR_PROCESSES; i++){
    sem_wait(finishing_sem);
    sem_wait(finishing_sem);
  }
  
   puts("Terminaron todos los hijos\n");
}
void check_result(){
  int correct_result;
  correct_result = OP_TIMES * TOTAL_PAIR_PROCESSES * (DEC_VALUE + INC_VALUE);
  puts("El resultado tenia que ser: ");
  printDec((uint64_t)correct_result);
  puts(" Y mi resultado dio: ");
  printDec((uint64_t)global);
  newline();
  if ( correct_result == global ){
    puts("COMO DICE SUSANA: CORRECTO!\n");
  }else
  {
    puts("ERROR EN LA SINCRONIZACION: INCORRECTO!\n");
  }
}
void test_sync(){
  test_setup(1);
  launch_processes();
  finishing();
  check_result();
  my_sem_close(finishing_sem);
  my_sem_close(sem_pointer);
  puts("Sems closed\n");
}

void test_no_sync(){
  test_setup(0);
  launch_processes();
  finishing();
  check_result();
  my_sem_close(finishing_sem);
  my_sem_close(sem_pointer);
  
}
