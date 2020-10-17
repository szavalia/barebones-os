#ifndef __USER_INTERRUPTS_H__
#define __USER_INTERRUPTS_H__
#include <stdint.h>
int int80_handler();

void sys_write(uint64_t  regs[]);
void sys_read(uint64_t  regs[]);
void sys_getReg(uint64_t  regs[]);
void sys_time(uint64_t  regs[]);
void sys_temp(uint64_t  regs[]);
void sys_getMem(uint64_t  regs[]);
void sys_cpuinfo(uint64_t  regs[]);
void sys_context(uint64_t  regs[]);
void sys_update_context(uint64_t  regs[]);
void sys_malloc(uint64_t  regs[]);
void sys_free(uint64_t  regs[]);
void sys_mem(uint64_t  regs[]);
void sys_kill(uint64_t  regs[]);
void sys_ps(uint64_t  regs[]);
void sys_loop(uint64_t  regs[]);
void sys_launch(uint64_t  regs[]);
void sys_pid(uint64_t regs[]);
void sys_exit(uint64_t  regs[]);
void sys_renounce(uint64_t  regs[]);
void sys_block(uint64_t regs[]);

extern uint_least64_t cpuTemperature();
extern uint8_t memContent(uint8_t * i);
extern void renounce();

#endif