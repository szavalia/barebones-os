
#include "test_mm.h"

#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 * 1024 //Should be around 80% of memory managed by the MM
#define NULL (void *) 0

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  int num_tests = 1;

  while (num_tests){
    puts("\nTest numero: ");
    printDec(num_tests);
    newline();
    num_tests++;
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = ltmalloc(mm_rqs[rq].size); // TODO: Port this call as required
      
      if(mm_rqs[rq].address == NULL){//TODO: check if NULL
        puts("DEVUELVE NULL EN ");
        printDec(rq);
        newline();
      }
      total += mm_rqs[rq].size;
      rq++;
    }
    puts("\nSALI DEL WHILE\n");
    // Set
    uint32_t i;
    for (i = 0; i < rq; i++){
      printDec(i);
      putChar(' ');
      if (mm_rqs[i].address != NULL){
        printHex(mm_rqs[i].address);
        putChar('\n');
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
      }              
    }
      
    puts("\nSALI DEL FOR 1\n");
    // Check
    for (i = 0; i < rq; i++){
      printDec(i);
      putChar(' ');
      if (mm_rqs[i].address != NULL){
        printHex(mm_rqs[i].address);
        putChar('\n');
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
          puts("ERROR!\n"); // TODO: Port this call as required
        }
      }
    }
      
    puts("\nSALI DEL FOR 2\n");    

    // Free
    for (i = 0; i < rq; i++){
      printDec(i);
      putChar(' ');
      if (mm_rqs[i].address != NULL){
        printHex(mm_rqs[i].address);
        putChar('\n');
        ltmfree(mm_rqs[i].address);  // TODO: Port this call as required
      }
    }
    puts("\nSALI DEL FOR 3\n");    
      
  } 
}




