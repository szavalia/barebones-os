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

void * getFunction( char * name){
	for ( int i = 0 ; i < NUM_COMMANDS; i++){
		if(strequals(commands[i].name, name)){
			return commands[i].func;
		}
	}
	return NULL;
}

void launchProcess(){
	char usr_command[COMMAND_BUFFER_SIZE];
	puts("Ingresa el nombre y los argumentos (como en bash!)\n");
	show_processed_scanf(usr_command, COMMAND_BUFFER_SIZE);
	char *argv[MAX_ARGS];
	char * token;
	*argv = strtok(usr_command, ' ');
	int i=1;
	do{	
		token = strtok(NULL, ' ');
		argv[i++] = token;
	}
	while(token != NULL && i < MAX_ARGS);
	argv[i] = NULL;
	void * funct = getFunction(*argv);
	if(funct != NULL){
		newline();
		callLaunch(funct, i, argv);
	}
	else{
		puts("\nNo existe tal funcion\n");
	}
	
}

void exit(){
	callExit();
}

void help(){

	for(int i=0; i < NUM_COMMANDS ; i++){
		puts("\t- ");
		puts(commands[i].name);
		puts(": ");
		puts(commands[i].desc);
	}
	return;
}

char * strcopy(char *destination, char *source)
{
    char *start = destination;

    while(*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0'; // add '\0' at the end
    return start;
}

void error(){
	int aux = 2/0;
}



void sh(){ 
		if(!buffer_initialized){
			initializeCommandVector();
			buffer_initialized = TRUE;
		}
		char usr_command[COMMAND_BUFFER_SIZE];
		while(1){
		puts("$ ");
		show_processed_scanf(usr_command, COMMAND_BUFFER_SIZE); 
		newline();

		for(int i=0; i<NUM_COMMANDS ; i++){
			if(strequals(usr_command, commands[i].name)){
				(*(commands[i].func))();
			}
		}
	}
	return;
}
