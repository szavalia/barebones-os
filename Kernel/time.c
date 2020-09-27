// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "time.h"
#include "video_driver.h"
#include "process.h"
static unsigned long ticks = 0;

void timer_handler( uint64_t stack_pointer) {
	ticks++;
	if(ticks %10 ==  0 ){
		switchProcess(scheduler(stack_pointer));
	}
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void printTime(){
	printDec(getHours());
	printS(":");
	printDec(getMinutes());
	printS(":");
	printDec(getSeconds());
}

void getTime(int * time){
	time[0] = getHours();
	time[1] = getMinutes();
	time[2] = getSeconds();
}
