#ifndef __USER_INTERRUPTS_H__
#define __USER_INTERRUPTS_H__
#include <stdint.h>
int int80_handler();

void sys_write();
void sys_read();
void sys_getReg();
void sys_time();
void sys_temp();
void sys_getMem();
void sys_cpuinfo();
void sys_context();
void sys_update_context();
void sys_malloc();
void sys_free();
void sys_mem();
void sys_kill();
void sys_ps();
void sys_loop();
void sys_launch(uint64_t stack_pointer);
void sys_pid();
void sys_exit();


extern uint_least64_t cpuTemperature();
extern uint8_t memContent(uint8_t * i);

#endif