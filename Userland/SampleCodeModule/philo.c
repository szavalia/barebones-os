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
    //puts("    Philosopher: ");
    //printDec(phnum + 1) ;
    //puts(" is eating a new plate\n");
}
void eat( int phnum){
    state[phnum] = EATING;
    /*puts("Philosopher ");
    printDec(phnum + 1);
    puts(" is Eating nom...\n");*//*
    printEating(phnum);
}*/
/*
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
    sem_post(mozo);*/
}




void testfork(int phnum) 
{ 
    if (state[phnum] == HUNGRY 
        && state[LEFT] != EATING 
        && state[RIGHT] != EATING) { 
        // state that eating 
        state[phnum] = EATING; 

        /*
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
        */ 
        sem_post(S[phnum]); 
    } 
} 
  
// take up chopsticks 
void take_fork(int phnum) 
{ 
  
    sem_wait(mozo); 
  
    // state that hungry 
    state[phnum] = HUNGRY; 
    /*
    puts("Philosopher ");
    printDec(phnum + 1);
    puts(" is Hungry\n");*/
    // eat if neighbours are not eating 
    testfork(phnum); 
  
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
  /*
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
 */
    
    testfork(LEFT); 
    testfork(RIGHT); 
  
    sem_post(mozo); 
} 
  
void* philosopher(int argc , char * argv[]) { 
    int i = (int) argv[1];
    int j;
    while (1) { 
         
        //puts("Im the philo number ");
        //printDec( i );
       // newline();
       // sleep(1); 
         renounceCPU();
       for( j = 0 ;j < 10; j++){
           //thinking
            
       }
        take_fork(i);
        printEating();
        put_fork(i);
        renounceCPU();
       //askFork(i);
  
       // sleep(0); 
  

    } 
} 

 
void* philosopherAdded(int argc , char * argv[]) { 
    int i = (int) argv[1];
    int j;
    int phnum= i;
       
          testfork(LEFT); 
          testfork(RIGHT); 
    while (1) { 
      
          renounceCPU();
       for( j = 0 ;j < 10; j++){
           //thinking
          
       }
        take_fork(i);
        printEating();
        put_fork(i);
        renounceCPU();
       //askFork(i);
  
       // sleep(0); 
  

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
        printDec( (long) i );
        callKill(children[i]);
    }
    callExit();

}

void addPhilo(char * philo_argv[][3] , int children[]){
      callLaunch ( philosopherAdded, 3 , philo_argv[philo_count], &(children[philo_count]) );
      philo_count++;
      /*
      puts("\nPhilosopher "); 
      printDec(++philo_count);
      puts(" is thinking\n");*/
}
void removePhilo( int children[]){
    philo_count--;
    state[philo_count] = THINKING;
    callKill(children[philo_count]);
    /*
    puts("\nPhilosopher: ");
    printDec(philo_count + 1 );
    puts(" has decided that he wants to die\n");
    */
}

int philosopher_problem( int argc , char * argv[]) {
    int proc_created[N];
    char proc_name[] = "philosopher";
    char ampersand[] = "&"; 
    char * philo_argv[N][3]; 
   // int numbers[N];
    int i; 
    //pthread_t thread_id[N]; 
    // initialize the semaphores 
    sem_init(&mozo, 1); 
    printHex(mozo);
    newline();
    for (i = 0; i < N; i++) {
       // numbers[i] = i+1;
        sem_init(&(S[i]), 0);
        puts(" my sem : ");
        printHex(S[i]);
        newline();
        phil[i] = i;
        state[i] = THINKING;
        fork_state[i] = 1;
        philo_argv[i][0] = proc_name;
        philo_argv[i][1] = (char *) i;
        philo_argv[i][2] = ampersand;
    }

    puts("ENTRANDO AL FOR\n");
    
    for (i = 0; i < philo_count; i++) { 
        
        // create philosopher processes
        callLaunch ( philosopher, 3 , philo_argv[i] , &(proc_created[i]) ); 
        /*
        puts("\nPhilosopher "); 
        printDec(i+1);
        puts(" is thinking\n");*/
    }
    listen(philo_argv , proc_created );
    puts("Kill the father pls\n");
    while(1){};
} 