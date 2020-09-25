// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "video_driver.h"
#include "keyboard.h"
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6
static void zero_division( uint64_t rip);
static void invalid_opcode( uint64_t rip);

extern uint64_t getRIP();

void exceptionDispatcher(int exception , uint64_t rip) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division(rip);
	else if(exception ==  INVALID_OPCODE_EXCEPTION_ID)
		invalid_opcode(rip);
}

static void zero_division( uint64_t rip) { //otorgar información sobre el tipo de error, instruction pointer y registros en el momento del error.
	saveRegs();      //me pisa los que tenía guardados por el usuario, pero bueno
	clear();
	printS("ERROR: division por cero\n");
	printS("RIP: ");
	printHex(rip);
	newline();
	printS("Registros:\n");
	printRegs(); 
	//haltcpu(); //ojo, hay que tener cuidado con el stack: si llamo a muchas interrupciones seguidas, tengo que limpiar el stack
}

static void invalid_opcode( uint64_t rip){
	saveRegs();      //me pisa los que tenía guardados por el usuario, pero bueno
	clear();
	printS("ERROR: invalid opcode\n");
	printS("RIP: ");
	printHex(rip);
	newline();
	printS("Registros:\n");
	printRegs(); 
}

