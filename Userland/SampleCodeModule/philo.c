#include "philo.h"
int philo_count = 6;
int state[N];
int phil[N]; 
int fork_state[N] = { 1 , 1 , 1 , 1 , 1};
int hungry = N;

void addPhilo(char * philo_argv[][3] , int children[]);
void removePhilo( int children[]);

void * mozo; 
void * S[N]; 
  
void printEating(){
   puts("| ");
    for( int i = 0; i < philo_count ; i++){
        if ( state[i] == EATING){
            puts("E ");
        }else{
            puts(". ");
        }
    }
    puts("|\n");
}
void eat( int phnum){
    state[phnum] = EATING;
}




void testfork(int phnum) 
{ 
    if (state[phnum] == HUNGRY 
        && state[LEFT] != EATING 
        && state[RIGHT] != EATING) { 
        state[phnum] = EATING; 
        sem_post(S[phnum]); 
    } 
} 
  
// take up chopsticks 
void take_fork(int phnum) 
{ 
  
    sem_wait(mozo); 
  
    // state that hungry 
    state[phnum] = HUNGRY; 
    
    // eat if neighbours are not eating 
    testfork(phnum); 
  
    sem_post(mozo); 
  
    // if unable to eat wait to be signalled 
    sem_wait(S[phnum]); 
  
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 
  
    sem_wait(mozo); 
  
    // state that thinking 
    state[phnum] = THINKING; 
    
    testfork(LEFT); 
    testfork(RIGHT); 
  
    sem_post(mozo); 
} 
  
void philosopher(int argc , char * argv[]) { 
    uint64_t i = (uint64_t) argv[1];
    int j;
    while (1) { 
         
        for( j = 0 ;j < 10; j++){
           //thinking            
       }
        take_fork(i);
        printEating();
        put_fork(i);
        renounceCPU();
    } 
} 

 
void philosopherAdded(int argc , char * argv[]) { 
    uint64_t i = (uint64_t) argv[1];
    int j;
    int phnum= i;
          testfork(LEFT); 
          testfork(RIGHT); 
    while (1) { 
        for( j = 0 ;j < 10; j++){
            //thinking
        }
        take_fork(i);
        printEating();
        put_fork(i);
        renounceCPU();
    } 
} 
void listen(char * philo_argv[][3] , int children[] ){
    char buffer[1];
    do{
        scanChar(buffer);
        switch (*buffer)
        {
        case 'a':
            if ( philo_count < N){
                addPhilo(philo_argv , children);
            }
            break;
        
        case 'r':
            if ( philo_count > 5){
                removePhilo(children);
            }
            break;
        }


    }while(*buffer != 'x');
    puts("\n");
    for( int i = 0 ; i < philo_count ; i++ ){
        puts("killing philo\n");
        printDec((uint64_t) i );
        callKill(children[i]);
    }
    callExit();

}

void addPhilo(char * philo_argv[][3] , int children[]){
      callLaunch ( philosopherAdded, 3 , philo_argv[philo_count], &(children[philo_count]) );
      philo_count++;
}
void removePhilo( int children[]){
    philo_count--;
    state[philo_count] = THINKING;
    callKill(children[philo_count]);
}

void philosopher_problem( int argc , char ** argv) {
    int proc_created[N];
    char proc_name[] = "philosopher";
    char ampersand[] = "&"; 
    char * philo_argv[N][3]; 
    int i; 
    // initialize the semaphores 
    sem_init(&mozo, 1); 
    printHex((uint64_t)mozo);
    newline();
    for (i = 0; i < N; i++) {
       // numbers[i] = i+1;
        sem_init(&(S[i]), 0);
        puts(" my sem : ");
        printHex((uint64_t)S[i]);
        newline();
        phil[i] = i;
        state[i] = THINKING;
        fork_state[i] = 1;
        philo_argv[i][0] = proc_name;
        philo_argv[i][1] = (char *) ((uint64_t) i);
        philo_argv[i][2] = ampersand;
    }

    puts("ENTRANDO AL FOR\n");
    
    for (i = 0; i < philo_count; i++) { 
        
        // create philosopher processes
        callLaunch ( philosopher, 3 , philo_argv[i] , &(proc_created[i]) ); 
    }
    listen(philo_argv , proc_created );
    puts("Kill the father pls\n");
    while(1){};
} 