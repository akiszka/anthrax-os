#include <types.hpp>

#include "keyboard.hpp"
#include "idt.hpp"
#include "cpu.hpp"

static u8 _scancode = 0;
static bool _waiting_for_keys = false;

__attribute__ ((interrupt)) void keyboard_ir(struct interrupt_frame* frame);

void keyboard::initialize(u16 code_selector) {
    idt::set_descriptor(idt::PIC1_IRQ_KEYBOARD, code_selector,
			idt::FLAG_TYPE_32B_INT | idt::FLAG_PRESENT,
		       keyboard_ir);
}

uint8_t keyboard::wait_for_key() {
    u8 result = 0;
    _waiting_for_keys = true;
    while ((result = scancode_to_ascii[_scancode]) == 0) { // wait for a printable char
	io_wait();
    }
    _scancode = 0; // reset the scancode
    _waiting_for_keys = false; // stop waiting
    return result;
}

__attribute__ ((interrupt))
void keyboard_ir(struct interrupt_frame* frame) {
    if (_waiting_for_keys) {
	if (in8(keyboard::REG_CMD_STATUS) & keyboard::FLAG_SCANCODE_READ) { // is there data to read
	    _scancode = in8(keyboard::REG_DATA);
	}
    }
    idt::pic_end_interrupt(idt::PIC1_IRQ_KEYBOARD);
}
