#include "terminal.h"
#include "test_sync.h"
typedef struct command_t{
	void (*func)(int, char **);
	char * name;
	char * desc;
} command_t;

static command_t commands[MAX_COMMANDS];
static int buffer_initialized=0;


static char * descriptions[] = { "te muestra opciones de ayuda\n", //help
"muestra la hora del sistema en formato HH:MM:SS\n",  //printTime
"muestra la marca y modelo de la cpu\n", 	//printCPUInfo
"muestra la temperatura del procesador\n",  //printTemp
"excepcion de division por 0\n", 	//divError
"excepcion de operacion invalida\n", //codeError
"imprime registros, guardar con Alt+R\n", //inforeg
"printea 32 bytes a partir de una direccion\n", //printmem
"imprime memoria dinamicamente asignada\n", //mem
"mata el proceso que le indiques\n", //kill
"lista los procesos\n", //ps 
"Finaliza el proceso actual\n", //exit
"Cambia el estado de un proceso entre bloqueado y listo dado su ID\n", //block
"Cambia la prioridad del proceso (parametros: PID y new_prio)\n", //nice
"Muestra el estado de los pipes\n", //pipe
"Consulta el estado de los semaforos\n", //semstate
NULL };


static void (*functions[])(int, char **) = {help, printTime, printCPUInfo, printTemp, divError, codeError, inforeg, printmem, mem, kill, ps, exit, block, nice, pipe, sem_state ,NULL};
static char * names[] = {"help","time","cpuinfo","cputemp","div","op","inforeg","printmem","mem","kill","ps", "exit", "block", "nice", "pipe", "semstate" ,NULL};

static command_t processes[MAX_PROCESSES];
static char * process_descriptions[] = {
"Imprime el PID actual junto con un saludo\n",//loop
 "lanza la terminal\n",//sh
 "Imprime stdin a pantalla\n", //cat 
"Cuenta cantidad de lineas de stdin\n", //wc
"Filtra vocales de stdin\n",//filter
"El problema de los filosofos", //philo
"Realiza un testeo de los semaforos\n",  //semtest
"Testeo de memoria\n", //memtest
NULL};  
static void (*process_functions[])(int, char **) = {loop, sh, cat, wc, filter, philosopher_problem, semtest, memtest, NULL};
static char * process_names[] = {"loop", "sh", "cat", "wc", "filter", "philo", "semtest", "memtest", NULL};



static uint64_t regs[16];
static char * regNames[] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","RSP","R8","R9","R10","R11","R12","R13","R14","R15"};


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

void ps(int argc, char ** argv){ 
	callPs();
}

void nice(int argc, char ** argv){
	if(argc != 3){
		return;
	}
	int pid = stringToNum(argv[1]);
	int new_prio = stringToNum(argv[2]);
	callNice(pid, new_prio);
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
		puts("- ");
		puts(commands[i].name);
		puts(": ");
		puts(commands[i].desc);
	}
	for(int j=0; processes[j].name != NULL; j++){
		puts("- ");
		puts(processes[j].name);
		puts(": ");
		puts(processes[j].desc);
	}
	newline();
	return;
}

void divError(int argc, char ** argv){
	int aux = 2/0;
}


int processInput(char ** argv_destination, int i){
	char * token;
	do{	
		token = strtok(NULL, ' ');
		argv_destination[i++] = token;
	}
	while(token != NULL && i < MAX_ARGS && *token != '|');
	return i-1; // No quiero que cuente el token que es pipe o NULL
}

void parse_command(){ 
	char * usr_command;
	usr_command = ltmalloc(COMMAND_BUFFER_SIZE);
	puts("$ ");
	show_processed_scanf(usr_command, COMMAND_BUFFER_SIZE);
	newline();

	char *argv1[MAX_ARGS];
	*argv1 = strtok(usr_command, ' ');

	int argc1 = processInput(argv1, 1);

	char *argv2[MAX_ARGS];
	int argc2 = processInput(argv2, 0);
	int pipeID;

	if(argc2 != 0){
		int id[2];
		pipeOpen(id);
		
		int launchedPid2 = search_for_run(argv2, argc2, usr_command);
		change_input(id[0], launchedPid2); 
		int launchedPid1 = search_for_run(argv1, argc1, usr_command);
		change_output(id[1], launchedPid1);
		
		if(launchedPid1 < 0 || launchedPid2 < 0){
			puts("No se puede pipear una built in\n");
			return;
		}
		
		
	}
	else{
		search_for_run(argv1, argc1, usr_command);
	}


}

int search_for_run(char * argv[], int argc, char * usr_command){
	for(int j=0; names[j] != NULL; j++){
		if(strequals(*argv, commands[j].name)){
			*argv = commands[j].name;
			(*(commands[j].func))(argc, argv);	
			return -1;
		}
	}	
	int launchedPid;
	for(int j=0; process_names[j] != NULL; j++){ //si es un proceso, lanzá uno nuevo
		if(strequals(*argv, processes[j].name)){
			*argv = processes[j].name;
			callLaunch(processes[j].func, argc, argv, &launchedPid); 
			return launchedPid;
		}
	}
	puts("No existe tal funcion\n");
	ltmfree(usr_command);
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

//alterna entre bloqueado y listo para un dado pid
void block(int argc, char **argv){
	if(argc != 2){
		puts("Error de parametro\n");
		return;
	}
	int pid = stringToNum(argv[1]);
	blockProcess(pid);
}

void pipe(int argc, char **argv){
	callPipe();
	return;
}







