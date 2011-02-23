#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vm.h"

enum modes {RUN_MODE, DUMP_MODE};

int filelength(FILE *fp) {
	if (fp == NULL)
		return -1;

	struct stat s;
	fstat(fileno(fp), &s);
	return (int) s.st_size;
}

int main(int argc, char **argv) {
	if (argc != 2 && argc != 3) {
		puts("vm file.bin");
		puts("vm -d file.bin");
		exit(0);
	}
	
	FILE *fp;
	int mode;

	if (strcmp(argv[1], "-d") == 0)
		mode = DUMP_MODE;
	else
		mode = RUN_MODE;
	
	if (mode == DUMP_MODE) 
		fp = fopen(argv[2], "r");
	else
		fp = fopen(argv[1], "r");

	if (fp == NULL)
		error("unable to find the requested file");

	int flen = filelength(fp);

	struct vm *v = alloc_vm(flen, 234);
	load_file(v, fp);

	printf("vm launched, memsize: %d, stacksize: %d\n", v->memsize, v->stacksize);
	
	if (mode == DUMP_MODE)
		dump_mem(v);
	else
		run(v);
	
}
