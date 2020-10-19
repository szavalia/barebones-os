#include "philo.h"
int state[N];
int phil[N]; 
int fork_state[N] = { 1 , 1 , 1 , 1 , 1};
int hungry = N;


void * mozo; 
void * S[N]; 
  
void printEating(int phnum){
    puts("| ");
    for( int i = 0; i < N ; i++){
        if ( state[i] == EATING){
            puts("E ");
        }else{
            puts(". ");
        }
    }
    puts("|\n");
    //puts("    Philosopher: ");
    //printDec(phnum + 1) ;
    //puts(" is eating a new plate\n");

}
void eat( int phnum){
    state[phnum] = EATING;
    /*puts("Philosopher ");
    printDec(phnum + 1);
    puts(" is Eating nom...\n");*/
    printEating(phnum);
}

void askFork(int phnum){
    sem_wait(mozo);
    if( fork_state[LEFT_FORK] == 1 && 
    fork_state[RIGHT_FORK] == 1 && 
        (state[phnum] == HUNGRY || 
        (state[LEFT] == THINKING 
            && state[RIGHT] == THINKING)
    )){
        fork_state[LEFT_FORK] = 0;
        fork_state[RIGHT_FORK] = 0;
        sem_post(mozo);
        eat(phnum);
        sem_wait(mozo); // le aviso al mozo que termine de comer;
        hungry-=1;
        state[phnum] = THINKING;
        if ( hungry == 0){
            //puts("So much thinking made us hungry\n");
            for( int i = 0 ; i < N ; i++){
                state[i] = HUNGRY;
            }
            hungry = N;
        }else{
           // puts("Phylosopher ");
            //printDec(phnum);
            //puts(" is thinking: Que es el infinito?\n");
        }
        fork_state[LEFT_FORK] = 1;
        fork_state[RIGHT_FORK] = 1;
    }
    sem_post(mozo);
}



/*
void testf(int phnum) 
{ 
    if (state[phnum] == HUNGRY 
        && state[LEFT] != EATING 
        && state[RIGHT] != EATING) { 
        // state that eating 
        state[phnum] = EATING; 
  
        // sleep(2); 
    
         puts("Philosopher ");
         printDec(phnum + 1);
         puts(" takes fork ");
         printDec(LEFT + 1);
         puts(" and ");
         printDec(phnum + 1);  
         puts("\n");   

  
         puts("Philosopher ");
         printDec(phnum + 1);
         puts(" is Eating\n");
        // sem_post(&S[phnum]) has no effect 
        // during takefork 
        // used to wake up hungry philosophers 
        // during putfork 
        sem_post(&S[phnum]); 
    } 
} 
  
// take up chopsticks 
void take_fork(int phnum) 
{ 
  
    sem_wait(&mozo); 
  
    // state that hungry 
    state[phnum] = HUNGRY; 
 
    puts("Philosopher ");
    printDec(phnum + 1);
    puts(" is Hungry\n");
    // eat if neighbours are not eating 
    testf(phnum); 
  
    sem_post(mozo); 
  
    // if unable to eat wait to be signalled 
    sem_wait(S[phnum]); 
  
    //sleep(1); 
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 
  
    sem_wait(mozo); 
  
    // state that thinking 
    state[phnum] = THINKING; 
  
    puts("Philosopher ");
    printDec(phnum + 1);
    puts(" putting fork ");
    printDec(LEFT + 1);
    puts(" and ");
    printDec(phnum + 1);  
    puts(" down\n");   

    puts("Philosopher ");
    printDec(phnum + 1); 
    puts(" is thinking\n");
 
    
    testf(LEFT); 
    testf(RIGHT); 
  
    sem_post(mozo); 
} */
  
void* philosopher(int argc , char * argv[]) { 
  
    while (1) { 
        
        long i = argc - 1; 
        //puts("Im the philo number ");
        //printDec( i );
       // newline();
       // sleep(1); 
        askFork(i);
  
       // sleep(0); 
  

    } 
} 
  
int philosopher_problem( int argc , char * argv[]) { 
    char * argv2[]= { "philosopher" , NULL};
   // int numbers[N];
    int i; 
    //pthread_t thread_id[N]; 
    // initialize the semaphores 
    sem_init(&mozo, 1); 
    printHex(mozo);
    sem_post(mozo);
    newline();
    for (i = 0; i < N; i++) {
       // numbers[i] = i+1;
        sem_init(&(S[i]), 0);
        printHex(S[i]);
        newline();
        phil[i] = i;
        state[i] = HUNGRY;
        fork_state[i] = 1;
    }
    
    for (i = 0; i < N; i++) { 
        
        // create philosopher processes
        callLaunch ( philosopher, i + 1 , argv2 ); 

        puts("Philosopher "); 
        printDec(i+1);
        puts(" is thinking\n");

    } 
    puts("Kill the father pls\n");
    while(1){};
} 