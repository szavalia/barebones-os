#include "terminal.h"
#define NUM_COMMANDS 17

typedef struct command_t{
	void (*func)(void);
	char * name;
	char * desc;
} command_t;

//static char * usr_command;
static command_t commands[NUM_COMMANDS];
static int buffer_initialized=0;
static char * names[] = {"help","time","cpuinfo","cputemp","div","op","inforeg","printmem","mem","launch","kill","ps","sh","loop", "exit", "cat", "wc"};
static char * descriptions[] = {"te muestra opciones de ayuda\n","muestra la hora del sistema en formato HH:MM:SS\n", "muestra la marca y modelo de la cpu\n", "muestra la temperatura del procesador\n", "excepcion de division por 0\n", "excepcion de operacion invalida\n", "imprime registros, guardar con Alt+R\n", "printea 32 bytes a partir de una direccion\n", "imprime memoria dinamicamente asignada\n", "lanza un proceso\n", "mata el proceso que le indiques\n", "lista los procesos\n", "lanza la terminal\n", "Imprime el PID actual junto con un saludo\n", "Finaliza el proceso actual\n", "Escribe a pantalla\n", "Cuenta cantidad de lineas en stdin\n"};
static void (*functions[])(void) = {help, printTime, printCPUInfo, printTemp, error, codeERROR, inforeg, printmemWrapper, mem, launchProcess, kill,ps,sh, loop, exit, cat, wc};

void initializeCommandVector(){
	for(int i=0; i<NUM_COMMANDS; i++){
		commands[i].name = names[i];
		commands[i].desc = descriptions[i];
		commands[i].func = functions[i];
	}
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

void ps(){ //FIXME: wrapper al pedo
	callPs();
}

void kill(int pid){
	char usr_command[NUM_BUFFER_SIZE];
	show_numeric_scanf(usr_command, NUM_BUFFER_SIZE);
	callKill(stringToNum(usr_command));
}

void bootMsg(){
	if(!buffer_initialized){
			//initializeCommandBuffer();
			initializeCommandVector();
			buffer_initialized = TRUE;
	}
	newline();
	puts("Estos son los comandos disponibles:\n");
	help();
	return;
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

void help(){
	for(int i=0; i < NUM_COMMANDS ; i++){
		puts("\t- ");
		puts(commands[i].name);
		puts(": ");
		puts(commands[i].desc);
	}
	return;
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

void cat(){
	char usr_command[COMMAND_BUFFER_SIZE];
	scanf_for_cat(usr_command, BUFFER_SIZE, 0); 
	newline();
}

void wc(){
	char usr_command[COMMAND_BUFFER_SIZE];
	int count = scanf_for_cat(usr_command, BUFFER_SIZE, 1); 
	newline();
	puts("cantidad de lineas:");
	printDec(count);
	newline();
}





