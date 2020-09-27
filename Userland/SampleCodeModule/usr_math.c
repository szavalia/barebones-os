#include "usr_math.h"
#define NUM_BUFFER_SIZE 20
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
extern void codeERROR();

// Function to convert hexadecimal to decimal, extraida de https://www.geeksforgeeks.org/program-for-hexadecimal-to-decimal/ 
int hexadecimalToDecimal(char hexVal[]) 
{    
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 

void printBase(uint64_t value, uint32_t base)
{
	int digits;
	char bufferNum[NUM_BUFFER_SIZE];
    digits = uintToBase(value, bufferNum, base);
	put(bufferNum, digits);
	return;
}

void printDec(uint64_t value){
    printBase(value, 10);
	return;
}

void printWithDecimals(double value){
	long  ent = parteEntera(value);
	printDec(ent);
	putChar('.');
	value-= ent;
	value *= 10000;
	long deci = parteEntera(value);
	printDec(deci);
}

void printHex(uint64_t value){
    printBase(value, 16);
	return;
}

void printBin(uint64_t value){
    printBase(value, 2);
	return;
}

void printReg(uint64_t value){
	char bufferNum[NUM_BUFFER_SIZE];
	int  digits = uintToBase(value,bufferNum,16);
	digits = 16-digits;
	while((digits--) > 0){
		putChar('0');
	}
	puts(bufferNum);
}

long parteEntera(uint64_t value){
	long rta = 0;
	long mult = 1;
	do{
		uint32_t remainder = value%10;
		rta+=  remainder * mult;
		mult *= 10;
	}
	while(value /= 10);
	return rta;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}