// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include "terminal.h"
#include "calculadora.h"
#include "usr_lib.h"
char * name = "BareUwUones terminal by LTM";

void launch(){
 char *argv[2];
	argv[0] = name;
	argv[1] = NULL;
	puts("Lanzando terminal\n");
	callLaunch( sh , 1 , argv, NULL);	
	return;	
}


void inactivity(){
	while(1){
		renounceCPU();
	}
}

int main() 
{
	launch();
	inactivity();
	
	return 0xDEADC0DE;
}

