#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vm.h"

int filelength(FILE *fp) {
	if (fp == NULL)
		return -1;

	struct stat s;
	fstat(fileno(fp), &s);
	return (int) s.st_size;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		puts("vm file.bin");
		exit(0);
	}
	
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL)
		error("unable to find the requested file");

	int flen = filelength(fp);

	struct vm *v = alloc_vm(flen, 234);
	load_file(v, fp);

	printf("vm launched, memsize: %d, stacksize: %d\n", v->memsize, v->stacksize);
	run(v);
	
}
