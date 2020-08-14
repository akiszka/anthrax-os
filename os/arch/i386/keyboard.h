#ifndef ARCH_I386_KEYBOARD_H
#define ARCH_I386_KEYBOARD_H

#define KEYBOARD_REG_CMD_STATUS 0x64
#define KEYBOARD_REG_DATA 0x60

#define KEYBOARD_FLAG_SCANCODE_READ 0x01 // if there is a scancode in the data reg, the lowest bit of the status reg will be set

void keyboard_initialize(uint16_t code_selector);
uint8_t keybord_wait_for_key();

#endif // ARCH_I386_KEYBOARD_H
