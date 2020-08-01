#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	__attribute__((__noreturn__))
	void abort(void);	
	char *convert(uint64_t num, int base);

#ifdef __cplusplus
}
#endif

#endif
