#include "usr_strings.h"
#include "usr_lib.h"
#define BUFFER_SIZE 1024
//static char * charBuffer;
//static int buffer_initialized=0; 
extern void codeERROR();

#define A 0x61
#define E 0x65
#define I 0x69
#define O 0x6F
#define U 0x75

#define EsVocal(c) (c=='A'||c=='a' || c=='E' || c=='e' ||c=='i' ||c=='I' || c=='O' || c=='o' || c=='U' || c=='u')



static char * strtokPointer = NULL;
static int strtokPosition = 0;
static int strLength = 0 ;

/*
static void initializeCharBuffer(){
	charBuffer=ltmalloc(NUM_BUFFER_SIZE);
}
*/

//FIXME: scanf con código repetido
void scanf(char * buffer, int size){
    int  current = 0;
	char character;
	char * charBuffer = &character;
    *charBuffer = 0;
    while( *charBuffer != '\n'){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
            buffer[current++] = *charBuffer;
        }        
    }
	buffer[current]='\0';
	return;
}

void show_scanf(char * buffer, int size){
    int  current = 0, deletes=0;
	char character;
	char * charBuffer = &character;
	*charBuffer = 0;
    while( *charBuffer != '\n' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
            buffer[current++] = *charBuffer;
			if(*charBuffer == '\b'){
				if(current-(deletes+1)>=0){ //para no borrar cosas anteriores
					deletes++;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current]='\0';
	return;
}

void show_processed_scanf(char * buffer, int size){
	int  current = 0;
	char character;
	char * charBuffer = &character;
	*charBuffer = 0;
    while( *charBuffer != '\n' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
			
			if(' ' <= *charBuffer && *charBuffer < 127 ){ //es una letra, número o signo de puntuación, '\b' = 127
				putChar(*charBuffer);
				buffer[current++] = *charBuffer;
			}
			else if(*charBuffer == '\t'){
				for(int i=0; i<5;i++){
					buffer[current++] = ' ';
				}
			}
			else if(*charBuffer == '\b'){
				if(current>0){ //para no borrar cosas anteriores
					current--;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current]='\0';
	return;
}

int scanf_for_cat(char * buffer, int size, int mode){
	int  current = 0;
	char character;
	char * charBuffer = &character;
	*charBuffer = 0;
	int count = 0;
    while( *charBuffer != 3 ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size && *charBuffer != 3){

			if(*charBuffer == '\n'){
				count++;
				newline();
			}
			if(*charBuffer == 3){
				putChar('c');
				return 0;
			}
			else if(' ' <= *charBuffer && *charBuffer < 127 ){ //es una letra, número o signo de puntuación, '\b' = 127
				//if(mode != 2 || (*charBuffer != A && *charBuffer != E && *charBuffer != I && *charBuffer != O && *charBuffer != U)){
				if(mode != 2 ||! EsVocal(*charBuffer)){
					putChar(*charBuffer);
					buffer[current++] = *charBuffer;
				}
			}
			else if(*charBuffer == '\t'){
				for(int i=0; i<5;i++){
					buffer[current++] = ' ';
				}
			}
			else if(*charBuffer == '\b'){
				if(current>0){ //para no borrar cosas anteriores
					current--;
					putChar(*charBuffer);
				}
			}
        }        
    }
	//putChar(' ');
	buffer[current]='\0';
	if(mode == 1){
		return count;
	}
	else
		return;
}

void scanf_for_calculator(char * buffer, int size){
	int  current = 0;
	char character;
	char * charBuffer = &character;
	*charBuffer = 0;
    while( *charBuffer != '=' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){
			
			if(' ' <= *charBuffer && *charBuffer < 127 ){ //es una letra, número o signo de puntuación, '\b' = 127
				putChar(*charBuffer);
				buffer[current++] = *charBuffer;
			}
			else if(*charBuffer == '\t'){
				for(int i=0; i<5;i++){
					buffer[current++] = ' ';
				}
			}
			else if(*charBuffer == '\b'){
				if(current>0){ //para no borrar cosas anteriores
					current--;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current-1]='\0';
	return;
}

void show_numeric_scanf(char * buffer, int size){
	int  current = 0;
	char character;
	char * charBuffer = &character;
	*charBuffer = 0;
    while( *charBuffer != '\n' ){
        scanChar(charBuffer);
        if(*charBuffer != 0 && current < size){	
			if('0' <= *charBuffer && *charBuffer <= '9' ){ //es una letra, número o signo de puntuación, '\b' = 127
				putChar(*charBuffer);
				buffer[current++] = *charBuffer;
			}
			else if(*charBuffer == '\b'){
				if(current>0){ //para no borrar cosas anteriores
					current--;
					putChar(*charBuffer);
				}
			}
        }        
    }
	buffer[current]='\0';
	return;
}

void puts(char * string){
	int length = strlen(string);
	put(string, length);
	return;
}

void putChar(char c){
	char charito;
    charito = c;
    put(&charito , 1);
	return;
}

void newline(){
	putChar('\n');
}

int strlen(char * string){
	int count = 0;
	while(string[count++] != 0);
	return count-1;
}

int strequals(char * s1, char * s2){
	int l1 = strlen(s1), l2=strlen(s2);
	int min = (l1<l2)? l1 : l2;
	if ( l1 != l2){
		return 0;
	}
	int equals = 1;
	for(int i=0; i < min && equals; i++){
		if(s1[i] != s2[i]){
			return 0;
		}
	}
	return 1;
}

char * strtok( char * string , char key ){ // UNA LOCURA, funciona como el stktok de verdad(algo asi)
        //aclaracion, si le paso NULL a string, no va a hacer lo de key
        //transforma el string a un "STREAM" o algo por el estilo, se modifica el string original
        //el strtok guarda de froma estatica el ultimo string que se le ingreso y sabe como iterar sobre sus tokens
		int i = 0;
        int lastPos;
        //strtokPointer y strtokPosition y strLength estan declaradas de forma estatica fuera del codigo
        if ( string != NULL ){
                //printf("cargando un pointer\n");
                strtokPointer = string;
                strtokPosition = 0;
                while ( strtokPointer[i] != 0 ){
                        if( strtokPointer[i] == key ){
                                strtokPointer[i] = '\0'; // primero reemplazo todos los 'key' con eof
                        }
                        i++;
                }
               // printf("el length en el strktok es %d\n", i );
                strLength = i; //me guardo el strlen 
        }
        if ( strtokPointer == NULL ){ // si no entro al if de arriba y no tenia nada da un NULL
                //printf("Debes cargar un pointer\n");
                return NULL;
        }

        do{ // abajo explico porque esto esta en un do while
            if ( ( i = strtokPosition) > strLength){ // seteo un index en la posicion que quedo mirando , si estoy mirando algo fuera del "STREAM" 
               // printf("ya llegue al final\n");
                strLength = 0; 
                strtokPosition =0; // este es el index actual sobre mi string
                strtokPointer= NULL; //llegue al final de mi string
                return NULL;
            }
            while( strtokPointer[i++] != 0 ){}
                lastPos = strtokPosition; //en donde empieza
                strtokPosition = i; // en donde el proximo
              //  printf("La position donde arranca el token es %d\n", lastPos);
        }
        while( *(strtokPointer+lastPos) == 0 ); //ahora me tengo que fijar, si el token que le voy a devolver seria string[] = "\0" , que seria un string vacion , no uno nulo, que no me sirve
        
        return strtokPointer+lastPos;       
}
