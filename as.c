/* a very crude assembler for the vm - it's one pass and
 * spits out error messages the ken thompson way
 * */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vm.h"
#include "tokens.h"
#include "error.h"

extern FILE* yyin;
extern char* yytext;
extern int yylex();
FILE *outfp;

int setup_parser(char *filename) {
	yyin = fopen(filename, "r");
	if (yyin == NULL)
		error("Unable to open input file");

	outfp = fopen("a.bin", "w+");
	if (outfp == NULL)
		error("Unable to open output file");

	return 0;
}

int parse() {
	int t;
	while((t = yylex()) != -1) {
		switch(t) {
			case T_PUSH:
				yylex();
				fputc(PUSH, outfp);
				fputc(atoi(yytext), outfp);
				break;

			case T_POP:
				yylex();
				fputc(POP, outfp);
				fputc(atoi(yytext), outfp);
				break;

			case T_DPRINT:
				fputc(DPRINT, outfp);
				break;

			case T_NOP:
				fputc(NOP, outfp);
				break;

			default:
				printf("unexpected %d in input", t);
				break;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		puts("as file.as");
		exit(0);
	}

	setup_parser(argv[1]);
	parse();
}

