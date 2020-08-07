#include "gdt.h"
#include "idt.h"
#include "timer.h"

void hal_system_startup() {
    gdt_initialize(); // set up the GDT
    idt_initialize(); // set up the IDT
    pit_initialize(); // set up the timer
    pic_initialize(); // set up the interrupt controller

    asm volatile ("sti":::"memory");
}

void hal_system_shutdown() {
    
}
