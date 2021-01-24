#include <stdio.hpp>
#include <stdarg.h>

#if defined(__is_libk)
#include <stdint.h>
#include <kernel/tty.hpp>
#endif

#if defined(__is_libk)
#include <kernel/tty.hpp>
#endif


int debug_printf(char *fmt, ...) {

#ifdef DEBUG

#if defined(__is_libk)
    uint8_t previous_color = terminal_getcolor();
    terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BROWN));
#endif // __is_libk

    printf("[dbg]");

    va_list ap;
    int r;

    va_start(ap, fmt);
    r = vsprintf(NULL, fmt, ap);
    va_end(ap);

#if defined(__is_libk)
    terminal_setcolor(previous_color);
#endif // __is_libk

    return r;

#else // DEBUG
    return 0;
#endif //DEBUG
}

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}

int puts(const char* string) {
	return printf("%s\n", string);
}
