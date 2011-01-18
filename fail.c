#include "fail.h"

void fail(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	puts("");
	va_end(ap);
	exit(EXIT_FAILURE);
}

