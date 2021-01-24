#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <kernel/hal.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/physical_memory.h>

#include <device/pci.h>

void kernel_main(multiboot_info_t* mbt, uint32_t magic) {
    terminal_initialize();

    if (MULTIBOOT_BOOTLOADER_MAGIC != magic) abort();

    hal_system_startup();
    pmmgr_initialize();

    printf("Anthrax OS booted by %s.\n", mbt->boot_loader_name);
    printf("Hello, kernel World!\n");

    printf("Available memory: %u MB\n", (mbt->mem_upper + mbt->mem_lower)/1024);

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

    pci_enumerate();
    pmmgr_complete_initialization();

    while(1) {
	printf("read key: %c\n", getch());
    }
}
