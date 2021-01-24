#include "gdt.hpp"
#include "idt.hpp"
#include "timer.hpp"
#include "keyboard.hpp"

#include <types.hpp>
#include <kernel/physical_memory.hpp>
#include <kernel/hal.hpp>

void hal::system_startup(multiboot_info_t* mbt) {
    gdt::initialize(); // set up the GDT
    u16 code_selector = gdt::get_selector(1, 0);

    idt::initialize(code_selector); // set up the IDT
    idt::pic_initialize(); // set up the interrupt controller
    timer::initialize(code_selector); // set up the timer

    keyboard::initialize(code_selector); // sey up the keyboard

    pmmgr::initialize();
    {
	multiboot_memory_map_t* entry = (multiboot_memory_map_t*) mbt->mmap_addr;
	while((address)entry < mbt->mmap_addr + mbt->mmap_length) {
	    // if the address is too big for 32-bit, just skip it
	    if (entry->addr > UINT32_MAX) {
		entry = (multiboot_memory_map_t*) ((address)entry + entry->size + sizeof(entry->size));
		continue;
	    }

	    address addr = entry->addr;
	    size_t size = entry->len;
	    if (entry->type == 1) {
		pmmgr::set_region_unused(addr, size);
	    } else {
		pmmgr::set_region_used(addr, size);
	    }

	    entry = (multiboot_memory_map_t*) ((address)entry + entry->size + sizeof(entry->size));
	}
    }
    pmmgr::complete_initialization();

    asm volatile ("sti":::"memory");
}

void hal::system_shutdown() {

}

uint8_t hal::getch() {
    return keyboard::wait_for_key();
}
