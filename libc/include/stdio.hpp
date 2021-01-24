#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.hpp>
#include <stdarg.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    int printf(char *fmt, ...);
    int sprintf(char *buf, char *fmt, ...);
    int vsprintf(char **out, char *format, va_list ap);
    int debug_printf(char *fmt, ...);
    int putchar(int);
    int puts(const char*);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _STDIO_H
