#include <types.hpp>

#include "keyboard.hpp"
#include "idt.hpp"
#include "cpu.hpp"

static u8 _scancode = 0;
static bool _waiting_for_keys = false;

static const char _scancode_to_ascii[0xff] = {
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

__attribute__ ((interrupt)) void keyboard_ir(struct interrupt_frame* frame);

void keyboard_initialize(u16 code_selector) {
    idt_set_descriptor(PIC1_IRQ_KEYBOARD, code_selector,
		       IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
		       keyboard_ir);
}

uint8_t keybord_wait_for_key() {
    u8 result = 0;
    _waiting_for_keys = true;
    while ((result = _scancode_to_ascii[_scancode]) == 0) { // wait for a printable char
	io_wait();
    }
    _scancode = 0; // reset the scancode
    _waiting_for_keys = false; // stop waiting
    return result;
}

__attribute__ ((interrupt))
void keyboard_ir(struct interrupt_frame* frame) {
    if (_waiting_for_keys) {
	if (in8(KEYBOARD_REG_CMD_STATUS) & KEYBOARD_FLAG_SCANCODE_READ) { // is there data to read
	    _scancode = in8(KEYBOARD_REG_DATA);
	}
    }
    pic_end_interrupt(PIC1_IRQ_KEYBOARD);
}
