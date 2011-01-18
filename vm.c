#include "vm.h"

struct vm* alloc_vm(int memsize, int stacksize)
{
	struct vm *v = calloc(1, sizeof(struct vm));
	if (v == NULL)
		fail("Unable to allocate virtual machine");

	v->mem = calloc(memsize, sizeof(char));
	if (v->mem == NULL)
		fail("Unable to allocate memory for vm");
	
	v->stack = calloc(stacksize, sizeof(char));
	if (v->stack == NULL)
		fail("Unable to allocate memory for vm stack");

	v->memsize = memsize;
	v->stacksize = stacksize;
	v->stackoffset = -1;
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
				if (v->stackoffset >= v->stacksize) 
					fail("stack overflow");
			
				v->stackoffset++;
				v->stack[v->stackoffset] = (int) ip;
				ip++;
				ip = v->stack + (*ip);
				break;

			case JMP:
				ip++;
				ip = v->stack + (*ip);
				break;

		}

		ip++;
	}
}
