// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "time.h"
#include "keyboard.h"
#include <stdint.h>
#include "user_interrupts.h"

static void int_20( uint64_t stack_pointer);
static void int_21();
static int int_80(uint64_t stack_pointer);

int irqDispatcher(uint64_t irq , uint64_t * stack_pointer) {
	switch (irq) {
		case 0:
			int_20(stack_pointer); //TimerTick
			break;
		case 1:
			int_21(); //Teclado
			break;
		case 60:
			return int_80(stack_pointer); //Int_80()
			break;
	}
	return 1;
}

void int_20( uint64_t stack_pointer) {
	timer_handler(stack_pointer);
}

void int_21(){
	keyboard_handler();
}

int int_80(uint64_t stack_pointer){
	return int80_handler(stack_pointer);
}


