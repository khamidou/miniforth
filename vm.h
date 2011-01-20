#ifndef __VM_H__
#define __VM_H__

#include <stdlib.h>
#include <stdio.h>

enum opcodes {NOP = 0, PUSH, POP, ADD, CALL, JMP, RET, DPRINT, INC, DEC};

struct vm {
	int *mem;
	int memsize;
	int *stack;
	int stacksize;
	int stackoffset;
	int *callstack;
	int callstacksize;
	int callstackoffset;
};

struct vm* alloc_vm(int memsize, int stacksize);

#endif
