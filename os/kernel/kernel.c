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

    hal_system_startup(mbt);

    printf("Anthrax OS booted by %s.\n", mbt->boot_loader_name);
    printf("Hello, kernel World!\n");

    printf("Available memory: %u MB\n", (mbt->mem_upper + mbt->mem_lower)/1024);

    pci_enumerate();

    while(1) {
	printf("read key: %c\n", getch());
    }
}
