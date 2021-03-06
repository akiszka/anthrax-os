#include <stdbool.h>
#include <types.hpp>
#include <string.hpp>

#include <kernel/tty.hpp>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static u16* const VGA_MEMORY = (u16*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static u8 terminal_color;
static u16* terminal_buffer;

void tty::initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
	    const size_t index = y * VGA_WIDTH + x;
	    terminal_buffer[index] = vga_entry(' ', terminal_color);
	}
    }
}

void tty::setcolor(u8 color) {
    terminal_color = color;
}

u8 tty::getcolor() {
    return terminal_color;
}

void tty::putentryat(unsigned char c, u8 color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void tty::newline(void) {
    for (size_t x = terminal_column; x < VGA_WIDTH; ++x) // erase the rest of the line
	putentryat(' ', 0, x, terminal_row);
    terminal_column = 0;

    // this is probably wrong and it breaks some memory...
    if (terminal_row == VGA_HEIGHT) { // at the end of the terminal, scroll up
	memcpy(terminal_buffer,
	       terminal_buffer + VGA_WIDTH,
	       VGA_WIDTH*VGA_HEIGHT*2);
    } else {
	++terminal_row;
    }
}

void tty::putchar(char c) {
    unsigned char uc = c;
    if (c == '\n') {
	newline();
	return;
    }

    putentryat(uc, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
	newline();
    }
}

void tty::write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
	putchar(data[i]);
}

void tty::writestring(const char* data) {
    write(data, strlen(data));
}
