#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"

#include <kernel/physical_memory.h>
#include <kernel/hal.h>

void hal_system_startup(multiboot_info_t* mbt) {
    gdt_initialize(); // set up the GDT

    uint16_t code_selector = gdt_get_selector(1, 0);

    idt_initialize(code_selector); // set up the IDT
    pic_initialize(); // set up the interrupt controller
    pit_initialize(code_selector); // set up the timer

    keyboard_initialize(code_selector); // sey up the keyboard

    pmmgr_initialize();
    {
	multiboot_memory_map_t* entry = (multiboot_memory_map_t*) mbt->mmap_addr;
	while((address)entry < mbt->mmap_addr + mbt->mmap_length) {
	    // if the address is too big for 32-bit, just skip it
	    if (entry->addr > UINT32_MAX) goto increment;

	    address addr = entry->addr;
	    size_t size = entry->len;
	    if (entry->type == 1) {
		pmmgr_set_region_unused(addr, size);
	    } else {
		pmmgr_set_region_used(addr, size);
	    }

	increment:
	    entry = (multiboot_memory_map_t*) ((address)entry + entry->size + sizeof(entry->size));
	}
    }
    pmmgr_complete_initialization();

    asm volatile ("sti":::"memory");
}

void hal_system_shutdown() {

}

uint8_t getch() {
    return keybord_wait_for_key();
}
