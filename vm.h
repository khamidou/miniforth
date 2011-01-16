#ifndef __VM_H__
#define __VM_H__

#include <stdlib.h>
#include <stdio.h>

enum opcodes {NOP = 0, PUSH, POP, ADD, CALL, RET, DPRINT};

struct vm {
	int *mem;
	int memsize;
	int *stack;
	int stacksize;
	int stackoffset;
};

struct vm* alloc_vm(int memsize, int stacksize);

#endif
