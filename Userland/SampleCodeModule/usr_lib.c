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

void loop(){
	int i=1;
	callLoop();
	while(i++>0){
		if(i % 100000000 == 0)
			callLoop();
	}
}	

void exit(){
	callExit(); //FIXME: wrapper al pedo
}












