#include "test_util.h"
#include "usr_lib.h"

   #define NULL (void *) 0 

//TO BE INCLUDED
void endless_loopproc(){
  while(1);
}

uint32_t my_create_processproc(char * name){
  int * pid;
  callLaunch(name,0,NULL, pid);
  return *pid;
}

uint32_t my_killproc(uint32_t pid){
  callKill(pid);
  return 0;
}

uint32_t my_blockproc(uint32_t pid){
  blockProcess(pid);
  return 0;
}

uint32_t my_unblockproc(uint32_t pid){
  blockProcess(pid);
  return 0;
}

#define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

enum State {ERROR, RUNNING, BLOCKED, KILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

void test_processes(){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1){

    // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
      p_rqs[rq].pid = my_create_processproc("endless_loop");  // TODO: Port this call as required

      if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
        puts("Error creating process\n");              
        return;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      for(rq = 0; rq < MAX_PROCESSES; rq++){
        action = GetUniform(2) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
              if (my_killproc(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                puts("Error killing process\n");       
                return;
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(my_blockproc(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                puts("Error blocking process\n");      
                return;
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }

      // Randomly unblocks processes
      for(rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
          if(my_unblockproc(p_rqs[rq].pid) == -1){            // TODO: Port this as required
            puts("Error unblocking process\n");         
            return;
          }
          p_rqs[rq].state = RUNNING; 
        }
    } 
  }
}

