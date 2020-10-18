// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include "terminal.h"
#include "calculadora.h"


static int context[1];
int main() 
{
	do{
	
		getContext(context);
		if(*context==0){
			sh(0, NULL);
		}
		else if(*context == 1){
			sh(0, NULL);
		}	
		changeContext();

	}
	while(1);

	
	//puts("I have decided that I want to die\n");
	return 0xDEADC0DE;
}

