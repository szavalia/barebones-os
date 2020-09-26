// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "usr_lib.h"
#include "usr_strings.h"
#define BUFFER_SIZE 1024
#define NULL (void *) 0 //FIXME: esto no debería estar incluido de algún lado?
//static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
static char usr_command[BUFFER_SIZE] = { 0 }; 
extern void codeERROR();
extern void * callMalloc(int size, void ** location);

uint64_t stringToNum(char * string){
	uint64_t result = 0;
	int length = strlen(string);
	for(int i=0; i<length; i++){
		result = result * 10 + ( string[i] - '0' );
	}
	return result;
}

void printTime(){
    int time[3];
	getTime(time);
	printDec(time[0]); //horas
	putChar(':');
	printDec(time[1]); //minutos
	putChar(':');
	printDec(time[2]); //segundos
	putChar('\n');
	return;
}

void inforeg(){ 
	uint64_t regs[16];
	char * regNames[] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","RSP","R8","R9","R10","R11","R12","R13","R14","R15"};
	getReg(regs);
	for(int i=0; i<16;i++){
		puts(regNames[i]);
		putChar(':');
		printReg((uint64_t)regs[i]);	
		putChar('\n');
	}
}

void printmem(uint8_t * dir){ 
	uint8_t bytes[32];
	getMem(dir, bytes);
	putChar('\n');
	for(int i = 0; i < 32; i++){
		printHex((long) dir+i );
		putChar(':');
		printHex(bytes[i]);
		putChar('\n');
	}

}

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

void printCPUInfo(){
	char vendor[13], brand[49];
	getCPUInfo(vendor, brand);
	puts("CPU Vendor: ");
	puts(vendor);
	newline();

	puts("CPU Brand: ");
	puts(brand);
	newline();

}

void printTemp(){
	uint64_t temp;
	getTemp(&temp);
	printDec(temp);
	newline();
}

void bootMsg(){
	newline();
	char msg[] = "Estos son los comandos disponibles:\n";
	puts(msg);
	help();
	return;
}

void help(){
	puts("    - help: te muestra opciones de ayuda\n");
	puts("    - inforeg: imprime registros, guardar con Alt+R\n");
	puts("    - time: muestra la hora del sistema en formato HH:MM:SS\n");
	puts("    - printmem: printea 32 bytes a partir de una direccion\n");
	puts("    - cpuinfo: muestra la marca y modelo de la cpu\n");
	puts("    - exit: cierra el programa\n");
	puts("    - cputemp: muestra la temperatura del procesador\n");
	puts("    - diverror: excepcion de division por 0\n");
	puts("    - invalid opcode: excepcion de operacion invalida\n");
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

int error(){
	int terror = 2/0;
	return -1;
}

void launch_terminal(){ 
		char memory[20] = { 0 };
		puts("$ ");
		show_processed_scanf(usr_command, 100); //no hay comandos más largos que 50 caracteres
		newline();
		if(strequals(usr_command, "help")){
			help();
		}
		else if(strequals(usr_command, "time")){
			printTime();
		}
		else if(strequals(usr_command, "inforeg")){
			inforeg();
		}
		else if(strequals(usr_command, "printmem")){
			puts("Inserte direccion de memoria (en decimal):\n");
			show_numeric_scanf(memory, 20); 
			uint64_t direc = stringToNum(memory);
			printmem((uint8_t *)direc);	
		}
		else if(strequals(usr_command, "cpuinfo")){
			printCPUInfo();
		}
		else if(strequals(usr_command, "diverror")){
			error();
		}
		else if(strequals(usr_command, "exit")){
			return;
		}
		else if(strequals(usr_command, "cputemp")){
			printTemp();
		}else if(strequals(usr_command, "invalid opcode")){
			codeERROR();
		}
		else {
			puts("Command not recognized\n");
		}
		
	return;
}


