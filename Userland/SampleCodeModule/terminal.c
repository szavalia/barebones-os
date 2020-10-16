#include "terminal.h"

typedef struct command_t{
	void (*func)(int, char **);
	char * name;
	char * desc;
} command_t;

static command_t commands[MAX_COMMANDS];
static int buffer_initialized=0;
static char * descriptions[] = {"te muestra opciones de ayuda\n","muestra la hora del sistema en formato HH:MM:SS\n", "muestra la marca y modelo de la cpu\n", "muestra la temperatura del procesador\n", "excepcion de division por 0\n", "excepcion de operacion invalida\n", "imprime registros, guardar con Alt+R\n", "printea 32 bytes a partir de una direccion\n", "imprime memoria dinamicamente asignada\n", "mata el proceso que le indiques\n", "lista los procesos\n", "Imprime stdin a pantalla\n", "Cuenta cantidad de lineas de stdin\n","Filtra vocales de stdin\n", "Finaliza el proceso actual\n", NULL};
static void (*functions[])(int, char **) = {help, printTime, printCPUInfo, printTemp, error, codeERROR, inforeg, printmem, mem, kill,ps, cat, wc, filter, exit, NULL};
static char * names[] = {"help","time","cpuinfo","cputemp","div","op","inforeg","printmem","mem","kill","ps", "cat", "wc","filter", "exit", NULL};

static command_t processes[MAX_PROCESSES];
static char * process_names[] = {"loop", "sh", NULL};
static char * process_descriptions[] = {"Imprime el PID actual junto con un saludo\n", "lanza la terminal\n", NULL};
static void (*process_functions[])(int, char **) = {loop, sh, NULL};

static void setupCalls(){
	for(int i=0; names[i] != NULL; i++){
		commands[i].name = names[i];
		commands[i].desc = descriptions[i];
		commands[i].func = functions[i];
	}

	for(int j=0; process_names[j] != NULL; j++){
		processes[j].name = process_names[j];
		processes[j].desc = process_descriptions[j];
		processes[j].func = process_functions[j];
	}
}

void printTime(int argc, char ** argv){
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

void inforeg(int argc, char ** argv){ 
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
void printmem(int argc, char ** argv){ 
    if(argc != 2){
        return;
    }
    char * hexDir = argv[1];
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

void printCPUInfo(int argc, char ** argv){
	char vendor[13], brand[49];
	getCPUInfo(vendor, brand);
	puts("CPU Vendor: ");
	puts(vendor);
	newline();

	puts("CPU Brand: ");
	puts(brand);
	newline();

}

void printTemp(int argc, char ** argv){
	uint64_t temp;
	getTemp(&temp);
	printDec(temp);
	newline();
}

void ps(int argc, char ** argv){ //FIXME: wrapper al pedo
	callPs();
}

void kill(int argc, char ** argv){
    if(argc != 2){
        return;
    }
    int pid = stringToNum(argv[1]);
	callKill(pid);
}


void help(int argc, char ** argv){
	for(int i=0; commands[i].name != NULL ; i++){
		puts("\t- ");
		puts(commands[i].name);
		puts(": ");
		puts(commands[i].desc);
	}
	for(int j=0; processes[j].name != NULL; j++){
		puts("\t- ");
		puts(processes[j].name);
		puts(": ");
		puts(processes[j].desc);
	}
	return;
}

void error(int argc, char ** argv){
	int aux = 2/0;
}

void parse_command(){ //TODO: bring her death
	char usr_command[COMMAND_BUFFER_SIZE];
	puts("$ ");
	show_processed_scanf(usr_command, COMMAND_BUFFER_SIZE);
	newline();
	char *argv[MAX_ARGS];
	char * token;
	*argv = strtok(usr_command, ' ');
	int i=1;
	do{	
		token = strtok(NULL, ' ');
		argv[i++] = token;
	}
	while(token != NULL && i < MAX_ARGS);
	int argc=i-1;

	for(int j=0; names[j] != NULL; j++){
		if(strequals(*argv, commands[j].name)){
			(*(commands[j].func))(argc, argv);	
			return;
		}
	}	

	for(int j=0; process_names[j] != NULL; j++){ //si es un proceso, lanzá uno nuevo
		if(strequals(*argv, processes[j].name)){		
			callLaunch(processes[j].func, argc, argv);
			return;
		}
	}
					
	
	puts("No existe tal funcion\n");
	
	
}

void sh(int argc, char ** argv){ 
	if(!buffer_initialized){
		setupCalls();
		buffer_initialized = TRUE;
	}
	while(1){
		parse_command();		
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

void filter(){
	char usr_command[COMMAND_BUFFER_SIZE];
	scanf_for_cat(usr_command, BUFFER_SIZE, 2); 
	newline();
}





