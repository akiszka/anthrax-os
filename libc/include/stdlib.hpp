#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.hpp>
#include <types.hpp>

#ifdef __cplusplus
extern "C" {
#endif

	__attribute__((__noreturn__))
	void abort(void);
	char *convert(u64 num, int base);

#ifdef __cplusplus
}
#endif

#endif
