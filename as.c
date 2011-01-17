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

/* labels are represented by a linked list */
struct label {
	char *name;
	int offset;
	struct label *next;
};

struct label* label_head;

struct label* alloc_label(char *name, int offset) {
	struct label* la = calloc(1, sizeof(struct label));

	if (la == NULL)
		error("Unable to allocate memory for one label");

	la->name = strdup(name);
	la->offset = offset;

	return la;
}

struct label* search_label(char *name) {
	struct label* l = label_head->next;

	while(l != NULL) {
		if (strcmp(name, l->name) == 0)
			return l;

		l = l->next;
	}
}

void append_label(struct label* l, struct label* head) {
	struct label *t = head->next;
	head->next = l;
	l->next = t;
}


/* writeint: write an int in a file by breaking it
 * in 8 chars
 */
int writeint(int val, FILE* fp) {

	if (fp == NULL)
		return -1;

	/* comes from:
	 * http://stackoverflow.com/questions/1105938/writing-and-reading-long-int-value-in-c-code/1105961#1105961
	 */

	unsigned char c1 =(val >>  0) & 0xff;
	unsigned char c2 = (val >>  8) & 0xff;
	unsigned char c3 = (val >> 16) & 0xff;
	unsigned char c4 = (val >> 24) & 0xff;

	fputc(c1, fp);
	fputc(c2, fp);
	fputc(c3, fp);
	fputc(c4, fp);

	return 0;
}

int parse() {
	int t;
	int current_offset = 0; /* the offset in the generated binary file */

	while((t = yylex()) != -1) {
		switch(t) {
			case T_PUSH:
				yylex();
				writeint(PUSH, outfp);
				writeint(atoi(yytext), outfp);
				current_offset++; /* 	we're pushing to values to the file, so update
							current_offset accordingly
							*/
				break;

			case T_POP:
				yylex();
				writeint(POP, outfp);
				writeint(atoi(yytext), outfp);
				break;

			case T_DPRINT:
				writeint(DPRINT, outfp);
				break;

			case T_NOP:
				writeint(NOP, outfp);
				break;

			case T_INC:
				writeint(INC, outfp);
				break;

			case T_DEC:
				writeint(DEC, outfp);
				break;

			default:
				printf("unexpected %d in input", t);
				break;
		}

		current_offset++;
	}
}

int main(int argc, char **argv)
{
	if (argc > 2) {
		puts("as file.as");
		exit(0);
	}

	if (argc == 1) {
		yyin = stdin;	
	} else {
		yyin = fopen(filename, "r");

		if (yyin == NULL)
			error("Unable to open input file");
	}

	outfp = fopen("a.bin", "w+");
	if (outfp == NULL)
		error("Unable to open output file");

	label_head = alloc_label("", 0);

	parse();
}

