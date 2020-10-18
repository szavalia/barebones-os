// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "calculadora.h"
#include "usr_lib.h"
#include <assert.h>
#define PLUS 0
#define MINUS 1
#define MULTIPLICATION 2
#define DIVISION 3
#define OPEN 4
#define CLOSE 5
#define NULL (void *) 0
#define EPSILON 0.000000000001

static char presedencia[5][6]={{ 1 , 1 , 0 , 0 , 0 , 1 },
                               { 1 , 1 , 0 , 0 , 0 , 1 },                         
                               { 1 , 1 , 1 , 1 , 0 , 1 },
                               { 1 , 1 , 1 , 1 , 0 , 1 },
                               { 0 , 0 , 0 , 0 , 0 , 0 }};

struct valueStack{
        double stack[30];
        int size;
};

struct operandStack{
        char stack[30];
        int size;
};

static struct valueStack vStack;
static struct operandStack oStack;
static int flag;
double decimalAtPlace( double num , double place){
    //simple , pone el numero /num/ en el digito numero /place/ despues de la coma
    //se asume que num esta entre 0 y 9
    while( place >0){
        num= num/10;
        place--;
    }
    return num;
}

int operandToNum( char c ){
        switch (c)
        {
        case '(':{
                        return OPEN;
                }

        case ')':{
                        return CLOSE;
                }
        case '+':{
                        return PLUS;
                }
        case '-':{
                        return MINUS;
                }
        case '/':{
                        return DIVISION;
                }
        case '*':{      
                        return MULTIPLICATION;
                }
        default:{
                //printf("error\n");
                flag = 5; //No existe el operando
                return 0;
                 }
        }
}


double apply( char c , double x , double y ){
        switch (c)
        {
        case '+':{
                        return x+y;
                }
        case '-':{
                        return x-y;
                }
        case '/':{
                        if ( y > -EPSILON && y < EPSILON ){
                                flag = 1; // division x 0 
                                //printf("divison por cero");
                                return 0;
                        }
                        return x / y;
                }
        case '*':{      
                        return x * y;
                }
        default:{
                flag = 5; // no existe el operando
                //printf("error\n");
                return 0;
                }
        }

}




int getPresedence( char actual , char peeked ){
        if(operandToNum(peeked) >= 5){
                return -1; //esto no va a pasar, pero lo pongo para que se vaya un warning
        }
        return presedencia[operandToNum(peeked)][operandToNum(actual)];
}






double thisAnum( char * string ){
        //se entra cuando se encuentra un string con mas de un numero
        int puntos = 0;//Flag de si encontre un punto
        int i = 0; //Index de mi string
        int signo = 1;// 1 si es positivo, -1 si es negativo

        if (string[i] == '-'){
                //printf("numero Negativo\n");
                signo=-1;
                i++;
        }
        double number= 0; //aca se va cargando el numero
        do
        {
                if (string[i] == '.'){
                        puntos++;
                }
                else if ( string[i] >= '0' && string[i]<='9')
                {
                        number= number*10; //dejo un espacio a la derecha
                        number+=(double) (string[i] - '0');         
                }
                else {
                       flag = 3;
                       //printf("encontre caracter invalido: %c\n", string[i]); //este seria el caracter invalido(testing)
                        return 0;
                }
                i++; // avanzo lo que miro

        }
        while( string[i] != 0 && puntos == 0 );

      //  printf("parte entera: %f \n",number);
     //   printf("mirando la posicion%d\n" , i);
        double decimal =0; // podria removerse ya 
        double aux;
        int d = 1;
        if ( puntos == 1 ){        
                while( string[i] != 0 ){
                   if ( string[i] >= '0' && string[i]<= '9'){
                       if ( d<7 ){//contame hasta la sexta coma, no entra mas en un double 
                       aux = string[i] - '0';
                       aux = decimalAtPlace( aux , d);
                      // printf( " agrego el decimal %f\n", aux);
                       decimal += aux; // si se remueve decimal , se reemplaza por number aca
                       }
                       d++;
                       i++;
                    }
                    else{
                       // printf("error en la coma %c\n" , string[i]);
                       flag = 3;
                        return 0;
                    }     
                }
                number+=decimal; // si se remueve decimal hagan lo que comente arriba  y esto lo sacan
        }
        number = number*signo; // por si es negativo
       // printf("agrego el numero %f\n", number);
        vStack.stack[vStack.size++]=number;
        return number; //de onda habria que ver si se guarda el numero en una posicion de memoria y se manda un flag de si ocurrio bien o mal
}



void processSingle( char c ){
        double x;
        double y;   
    //ya faltaria ver que hacer para pasarlo a postfija o que
        if ( c == '('){
                oStack.stack[oStack.size++] = c;
                return;            
        }
        else if (c ==')'){   
                if (oStack.size>0){
                        while( oStack.stack[oStack.size-1] != '('){
                                if ( oStack.size == 1){
                                        flag = 6; //missing '(' falta parentesis cerrando
                                        return;
                                }
                                if ( vStack.size < 2 ){
                                        flag = 2; //faltan numeros
                                        return;
                                }
                                y = vStack.stack[--vStack.size];
                                x = vStack.stack[--vStack.size];
                                x = apply(oStack.stack[--oStack.size], x , y );
                                vStack.stack[vStack.size++] = x;
                        }
                        oStack.size--; //pop ')' 
                }
                else{
                        flag = 6; //missing'('
                        return;           
                }
        
        }
        else if( c == '+' || c == '-' || c == '*' || c =='/'){
          
                while ( oStack.size > 0 && getPresedence(c , oStack.stack[oStack.size-1])){
        
                if ( vStack.size < 2){
                    flag = 2; // faltan numeros
                    return;
                } 
                y = vStack.stack[--vStack.size];
                x = vStack.stack[--vStack.size];
                x = apply(oStack.stack[--oStack.size], x , y );
                vStack.stack[vStack.size++]=x;
                }
                oStack.stack[oStack.size++] = c;
       }
        else if ( c >='0' && c <='9'){
        vStack.stack[vStack.size++]= (double) (c - '0');
      //  printf("singleNumber: %c\n" , c);
     }
     else
     {
             flag = 3; //caracter invalido
            // printf("error caracter:%c\n",c);
             return;
     }   
}
        
int calculate( char * input , double * answer ){
        flag = 0;    
        vStack.size = 0;
        oStack.size = 0; 
        double x , y ;
        char * token = strtok( input , ' ' );
        while ( token != NULL && flag == 0) {
                // printf("El numero que deberia obtener es %s o un error\n" , token);
                if( token[1]==0 ){ //si en la segunda posicion del string hay un '\0' entonces tiene un solo char
                        processSingle(token[0]); //proceso solo ese char
                }
                else
                {
                        //sino deberia asumirse que e sun numero
                        thisAnum( token );
                }
        
                token = strtok(NULL, ' ' );
                if ( flag != 0 ){
                        return flag;
                }
    }
        while(oStack.size > 0){
                if ( vStack.size<2 ){
                        //printf("faltan numeros\n");
                        flag = 2; //faltan numeros
                        return flag;
                }
                y = vStack.stack[--vStack.size];
                x = vStack.stack[--vStack.size];
                x = apply(oStack.stack[--oStack.size], x , y );
                vStack.stack[vStack.size++] = x;
        }       
        if (vStack.size != 1){
                // printf("sobraron numeros\n");
                flag = 4; // sobran numeros o faltan operandos
                return flag;
        }
        *answer = vStack.stack[vStack.size-1];
        //  printf("VALOR FINAL: %g\n", vStack.stack[0]);
        return flag;
}


void readFlag( int flag ){
        switch (flag)
        {
        case 1:
                puts("ERROR: DIVISION POR CERO\n");
                break;
        case 2:
                puts("ERROR: FALTAN NUMEROS\n");
                break;
        case 3:
                puts("ERROR: CARACTER INVALIDO");
                break;
        case 4:
                puts("ERROR: SOBRA/FALTA UN NUMERO");
                break;
        case 5:
                puts("ERROR: OPERANDO NO RECONOCIDO"); //Seria imposible entrar desde la calculadora, solo desde la funcion
                break;
        case 6:
                puts("ERROR: FALTAN PARENTESIS");
                break;
        default:
                break;
        }

}
static int called = 0;
void launch_calculator(){
        char input[100] = { 0 };	
	double answer;
	int flag = 0;
        if (called == 0){
                puts("Bienvenido a la calculadora\n");
                called = 1;
        }
                puts("EXPRESSION: ");
		scanf_for_calculator(input,100);
		flag = calculate(input,&answer);
		newline();
                puts("rta: ");
		printWithDecimals(answer);
		putChar('\n');
                readFlag(flag);
                newline();    
}
