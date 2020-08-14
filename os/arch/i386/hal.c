#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"

void hal_system_startup() {
    gdt_initialize(); // set up the GDT
    
    uint16_t code_selector = gdt_get_selector(1, 0);
    
    idt_initialize(code_selector); // set up the IDT
    pic_initialize(); // set up the interrupt controller
    pit_initialize(code_selector); // set up the timer

    keyboard_initialize(code_selector); // sey up the keyboard
    
    asm volatile ("sti":::"memory");
}

void hal_system_shutdown() {
    
}

uint8_t getch() {
    return keybord_wait_for_key();
}
