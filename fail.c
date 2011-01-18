#include "error.h"

void error(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	puts("");
	fflush(stdout);
	exit(EXIT_FAILURE);
}

