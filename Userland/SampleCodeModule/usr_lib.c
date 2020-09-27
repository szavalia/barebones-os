// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "usr_lib.h"
extern void callMalloc(int size, void ** location);
extern void callFree(void * pointer);
extern void callPs();
extern void callKill(int pid);
extern void callLaunch( void * process , int argc , char * argv[] );

typedef struct command_t{
	void (*func)(void);
	char * name;
	char * desc;
} command_t;

static char * usr_command;
static command_t commands[NUM_COMMANDS];
static char * names[] = {"help","time","cpuinfo","cputemp","div","op","inforeg","printmem","mem","launch","kill","ps"};
static char * descriptions[] = {"te muestra opciones de ayuda\n","muestra la hora del sistema en formato HH:MM:SS\n", "muestra la marca y modelo de la cpu\n", "muestra la temperatura del procesador\n", "excepcion de division por 0\n", "excepcion de operacion invalida\n", "imprime registros, guardar con Alt+R\n", "printea 32 bytes a partir de una direccion\n", "imprime memoria dinamicamente asignada\n", "lanza un proceso\n", "mata el proceso que le indiques\n", "lista los procesos\n"};
static void (*functions[])(void) = {help, printTime, printCPUInfo, printTemp, error, codeERROR, inforeg, printmemWrapper, mem, launchProcess, kill,ps};
static int buffer_initialized=0;

void initializeCommandVector(){
	for(int i=0; i<NUM_COMMANDS; i++){
		commands[i].name = names[i];
		commands[i].desc = descriptions[i];
		commands[i].func = functions[i];
	}
}

static void initializeCommandBuffer(){
	usr_command = ltmalloc(COMMAND_BUFFER_SIZE); 
}

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

//toma una dirección de memoria en hexa y devuelve los proximos 32 bytes
void printmem(char * hexDir){ 
	int dir = hexadecimalToDecimal( hexDir); 
	uint8_t bytes[32];
	getMem((uint8_t *)dir, bytes); //FIXME: casteos raros
	putChar('\n');
	for(int i = 0; i < 32; i++){
		printHex((long) dir+i );
		putChar(':');
		printHex(bytes[i]);
		putChar('\n');
	}

}
void printmemWrapper(){
	char memory[NUM_BUFFER_SIZE] = { 0 };
	puts("Inserte direccion de memoria (en hexa):\n");
	show_processed_scanf(memory, NUM_BUFFER_SIZE); 
	printmem(memory);	
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

void ps(){
	callPs();
}

void kill(int pid){
	show_numeric_scanf(usr_command, NUM_BUFFER_SIZE);
	callKill(stringToNum(usr_command));
}

void bootMsg(){
	if(!buffer_initialized){
			initializeCommandBuffer();
			initializeCommandVector();
			buffer_initialized = TRUE;
	}
	newline();
	puts("Estos son los comandos disponibles:\n");
	help();
	return;
}

void launchProcess(){
	show_processed_scanf(usr_command, COMMAND_BUFFER_SIZE);
	char *argv[20]; //FIXME: magic number, pasar a macro
	char * token;
	int i;
	do{
		token = strtok(usr_command, ' ');
		argv[i++] = token;
	}
	while(token != NULL && i < 20);
	callLaunch(usr_command, i, argv);
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



void launch_terminal(){ 
		if(!buffer_initialized){
			initializeCommandBuffer();
			initializeCommandVector();
			buffer_initialized = TRUE;
		}
		puts("$ ");
		show_processed_scanf(usr_command, COMMAND_BUFFER_SIZE); 
		newline();

		for(int i=0; i<NUM_COMMANDS ; i++){
			if(strequals(usr_command, commands[i].name)){
				(*(commands[i].func))();
			}
		}
		
	return;
}




