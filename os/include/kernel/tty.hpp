#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <types.hpp>

namespace tty {
    enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
    };

    void initialize(void);
    void setcolor(uint8_t color);
    u8 getcolor();
    void putchar(char c);
    void write(const char* data, size_t size);
    void writestring(const char* data);

    void putentryat(unsigned char c, u8 color, size_t x, size_t y);
    void newline(void);

    static inline u8 vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
    }

    static inline u16 vga_entry(unsigned char uc, u8 color) {
	return (u16) uc | (u16) color << 8;
    }
}

#endif
