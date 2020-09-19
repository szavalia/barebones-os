// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include "usr_lib.h"
#include "calculadora.h"



static int context[1];
int main() 
{
	bootMsg();

	
	do{
	
		getContext(context);
		if(*context==0){
			launch_terminal();
		}
		else if(*context == 1){
			launch_calculator();
		}	
		changeContext();

	}
	while(1);
	
	

	
	//puts("I have decided that I want to die\n");
	return 0xDEADC0DE;
}

