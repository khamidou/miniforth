#include "vm.h"

struct vm* alloc_vm(int memsize, int stacksize)
{
	int callstacksize = stacksize;

	struct vm *v = calloc(1, sizeof(struct vm));
	if (v == NULL)
		fail("Unable to allocate virtual machine");

	v->mem = calloc(memsize, sizeof(char));
	if (v->mem == NULL)
		fail("Unable to allocate memory for vm");
	
	v->stack = calloc(stacksize, sizeof(char));
	if (v->stack == NULL)
		fail("Unable to allocate memory for vm stack");

	v->callstack = calloc(callstacksize, sizeof(char));
	if (v->callstack == NULL)
		fail("Unable to allocate memory for vm call stack");

	v->memsize = memsize;
	v->stacksize = stacksize;
	v->stackoffset = -1;
	v->callstacksize = callstacksize;
	v->callstackoffset = -1;
	return v;
}

int load_file(struct vm *v, FILE *fd) 
{
	if (fd == NULL || v == NULL)
		fail("Unable to load file");
	
	fread(v->mem, sizeof(int), v->memsize, fd);
}

int run(struct vm *v)
{
	if (v == NULL)
		return -1;

	int *ip = v->mem;
	while(ip < v->mem + v->memsize) {
		switch(*ip) {
			case NOP:
				break;

			case PUSH:
				if (v->stackoffset >= v->stacksize) 
					fail("stack overflow");
			
				ip++;
				v->stackoffset++;
				v->stack[v->stackoffset] = *ip;
				break;

			case POP:
				if (v->stackoffset < 0)
					fail("stack underflow");

				v->stackoffset--;
				break;

			case DPRINT:
				/* a debug instruction - print the top of the stack */
				printf("%d\n", v->stack[v->stackoffset]);
				break;

			case INC:
				v->stack[v->stackoffset]++;
				break;

			case DEC:
				v->stack[v->stackoffset]--;
				break;

			case CALL:
				/* save the previous instruction pointer */
				if (v->callstackoffset >= v->callstacksize) 
					fail("call stack overflow");
			
				v->callstackoffset++;
				v->callstack[v->callstackoffset] = (int) ip;
				ip++;
				ip = v->mem + (*ip) - 1;
				break;

			case RET:
				/* get ip from the stack. it must be the thing just before
				 * the top of the stack - it implies that programs must return
				 * one and only one return value
				 */
				if (v->callstackoffset < 0) 
					fail("call stack underflow");
			
				ip = v->callstack[v->callstackoffset];
				v->callstackoffset--;
				break;

			case JMP:
				ip++;
				ip = v->mem + (*ip) - 1; /* because the pointer gets automatically incremented */
				break;

		}

		ip++;
	}
}
