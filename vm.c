#include "vm.h"

struct vm* alloc_vm(int memsize, int stacksize)
{
	struct vm *v = calloc(1, sizeof(struct vm));
	if (v == NULL)
		error("Unable to allocate virtual machine");

	v->mem = calloc(memsize, sizeof(int));
	if (v->mem == NULL)
		error("Unable to allocate memory for vm");
	
	v->stack = calloc(stacksize, sizeof(int));
	if (v->stack == NULL)
		error("Unable to allocate memory for vm stack");

	v->memsize = memsize;
	v->stacksize = stacksize;
	return v;
}

int load_file(struct vm *v, FILE *fd) 
{
	if (fd == NULL || v == NULL)
		error("Unable to load file");
	
	fread(v->mem, sizeof(char), v->memsize, fd);
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
				puts("ho");
				if (v->stackoffset >= v->stacksize) 
					error("stack overflow");
			
				ip++;
				printf("%d", *ip);
				v->stack[v->stackoffset] = *ip;
				v->stackoffset++;
				break;

			case POP:
				if (v->stackoffset == 0)
					error("stack underflow");

				v->stackoffset--;
				break;

			case DPRINT:
				/* a debug instruction - print the top of the stack */
				printf("%d\n", v->stack[v->stackoffset]);
				break;
		}

		ip++;
	}
}
