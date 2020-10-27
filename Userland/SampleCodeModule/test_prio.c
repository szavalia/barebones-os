#include "test_prio.h"
#include "usr_lib.h"

#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

char *argv[] = { "endless_loop" , "&" , NULL};

uint64_t my_getpid(){
  int pid;
  getPID(&pid);
  return pid;
}

void my_nice(uint64_t pid, uint64_t newPrio){
  callNice(pid, newPrio);
}

void my_kill(uint64_t pid){
  callKill(pid);
}

void my_block(uint64_t pid){
  blockProcess(pid);
}

void my_unblock(uint64_t pid){
  blockProcess(pid);
}

void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop(){
  uint64_t pid = my_getpid();

  while(1){
    printDec(pid);
    bussy_wait(MINOR_WAIT);
  }
}

uint32_t my_create_process2(){
  int pid;
  callLaunch(endless_loop, 2, argv, &pid);
  return pid;
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process2();

  bussy_wait(WAIT);
  puts("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        my_nice(pids[i], 1); //lowest priority 
        break;
      case 1:
        my_nice(pids[i], 2); //medium priority
        break;
      case 2:
        my_nice(pids[i], 3); //highest priority
        break;
   
    }
  }

  bussy_wait(WAIT);
  puts("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  puts("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        my_nice(pids[i], 2); //medium priority 
        break;
      case 1:
        my_nice(pids[i], 2); //medium priority
        break;
      case 2:
        my_nice(pids[i], 2); //medium priority
        break;
    }
  }

  puts("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  puts("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}

void priotest(int argc, char ** argv){
	puts("Test de prioridades\n");
	test_prio();
	newline();
	callExit();
}