#include <stdio.hpp>
#include <stdlib.hpp>

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	printf("kernel: panic: abort()\n");
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}

char *convert(u64 num, int base) {
	static char representation[]= "0123456789ABCDEF";
	static char buffer[65];
	char *ptr;

	ptr = &buffer[64];
	*ptr = '\0';

	do {
		*--ptr = representation[num%base];
		num /= base;
	} while(num != 0);

	return(ptr);
}
