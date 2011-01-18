/* a very crude assembler for the vm - 
 * it spits out fail messages the ken thompson way
 * */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "vm.h"
#include "tokens.h"
#include "fail.h"

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
		fail("Unable to allocate memory for one label");

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
	return fwrite(&val, sizeof(int), 1, fp);
}

/* the first pass only scans the file for labels */
int first_pass() {
	int t;
	int current_offset = 0; /* the offset in the generated binary file */
	int s;
	struct label *l;

	while((t = yylex()) != -1) {
		switch(t) {
			case T_LABEL:
				s = strlen(yytext);
				yytext[s-1] = '\0'; /* get rid of the ':' */

				l = alloc_label(yytext, current_offset);
				append_label(l, label_head);
				break;
			default:
				current_offset++;
				break;
		}
	}
}

int parse() {
	int t;
	int current_offset = 0; /* the offset in the generated binary file */
	int s;
	struct label *l;

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

			case T_LABEL:
				break;

			case T_CALL:
				yylex();
				l = search_label(yytext);
				if (l == NULL)
					fail("the label %s doesn't exists", yytext);

				writeint(CALL, outfp);
				writeint(l->offset, outfp);
				break;

			case T_JMP:
				yylex();
				l = search_label(yytext);
				if (l == NULL)
					fail("the label %s doesn't exists", yytext);

				writeint(JMP, outfp);
				writeint(l->offset, outfp);
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
	if (argc != 2) {
		puts("as file.as");
		exit(0);
	}

	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
		error("Unable to open input file");

	outfp = fopen("a.bin", "w+");
	if (outfp == NULL)
		error("Unable to open output file");

	label_head = alloc_label("", 0);

	first_pass();
	fclose(yyin);
	fopen(argv[1], "r");
	yyrestart(yyin);
	parse();
}

