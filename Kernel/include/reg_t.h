#include <stdint.h>
#ifndef REG_T_H_
#define REG_T_H_

#define REGISTERS 18
#define R15 0
#define R14 1
#define R13 2
#define R12 3
#define R11 4
#define R10 5
#define R9 6
#define R8 7
#define RSI 8
#define RDI 9
#define RBP 10
#define RDX 11
#define RCX 12
#define RBX 13
#define RAX 14
#define RIP 15
#define CS 16
#define FLAGS 17

typedef struct reg_t{
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t flags;
}reg_t;

#endif