// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "usr_lib.h"



void * ltmalloc(int size){
	
	if(size < 0){
		return NULL;
	}
	
	void * location;
	callMalloc(size, &location); //le tengo que pasar un int * asi que lo desreferencio ???
	return location;
}

void ltmfree(void * pointer){
	callFree(pointer);
}

void loop(int argc, char **argv){
	int i=1, greets = 0;
	newline();
	callLoop();
	while(i++>0 && greets <= 3){
		if(i % 100000000 == 0){
			newline();
			callLoop();
			greets++;
		}
	}
	callExit();
}	

void exit(int argc, char **argv){
	callExit();
}

void cat(int argc, char ** argv){
	char usr_command[BUFFER_SIZE];
	scanf_for_cat(usr_command, BUFFER_SIZE, 0); 
	newline();
	callExit();
}

void wc(int argc, char ** argv){
	char usr_command[BUFFER_SIZE];
	int count = scanf_for_cat(usr_command, BUFFER_SIZE, 1); 
	newline();
	puts("cantidad de lineas:");
	printDec(count);
	newline();
	callExit();

}

void filter(int argc, char ** argv){
	char usr_command[BUFFER_SIZE];
	scanf_for_cat(usr_command, BUFFER_SIZE, 2); 
	newline();
	callExit();
}

void memtest(int argc, char **argv){
	test_mm();
	puts("TERMINO EL TEST\n");
	callExit();
}

void semtest( int argc , char **argv){
	puts("Test sin sincronizacion\n");
	test_no_sync();
	newline();
	puts("Test con sincronizacion\n");
	test_sync();
	newline();
	callExit();
}














