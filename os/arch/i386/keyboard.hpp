#ifndef ARCH_I386_KEYBOARD_H
#define ARCH_I386_KEYBOARD_H

#include <types.hpp>

namespace keyboard {
    constexpr u8 REG_CMD_STATUS = 0x64;
    constexpr u8 REG_DATA = 0x60;

    constexpr u8 FLAG_SCANCODE_READ = 0x01; // if there is a scancode in the data reg, the lowest bit of the status reg will be set

    void initialize(u16 code_selector);
    u8 wait_for_key();

    constexpr char scancode_to_ascii[0xff] = {
	0, 0x1B /* ESC */,
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', 0x08 /* BACKSPACE */, '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'[', ']', '\n', 0 /* LCTRL pressed */,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	';', '\'', '`', 0 /* LSHIFT pressed */, '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm',
	',', '.', '/', 0 /* RSHIFT pressed */,
	'*', 0 /* LALT pressed */, ' ', 0 /* CAPSLOCK */,
	0 /* F1 */, 0 /* F2 */, 0 /* F3 */, 0 /* F4 */, 0 /* F5 */,
	0 /* F6 */, 0 /* F7 */, 0 /* F8 */, 0 /* F9 */, 0 /* F10 */,
	0 /* NUMLOCK pressed */, 0 /* SCROLLLOCK pressed */,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
	0, 0, 0, 0,
	0 /* F11 */, 0 /* F12 */

	/* to be continued... */
	// TODO: make scancodes for keys with SHIFT and ALT pressed
	// TODO: finish scancodes for released keys and media keys
    };
}

#endif // ARCH_I386_KEYBOARD_H
