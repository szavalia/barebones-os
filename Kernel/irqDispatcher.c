// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "time.h"
#include "keyboard.h"
#include <stdint.h>
#include "user_interrupts.h"

static void int_20();
static void int_21();
static void int_80();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20(); //TimerTick
			break;
		case 1:
			int_21(); //Teclado
			break;
		case 60:
			int_80(); //Int_80()
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}

void int_80(){
	int80_handler();
}


