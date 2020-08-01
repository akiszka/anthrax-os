#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

#define printf simple_printf
#define sprintf simple_sprintf
	int simple_printf(char *fmt, ...);
	int simple_sprintf(char *buf, char *fmt, ...);
	int putchar(int);
	int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif
