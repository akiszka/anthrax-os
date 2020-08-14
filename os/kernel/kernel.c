#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <kernel/hal.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>

void kernel_main(multiboot_info_t* mbt, uint32_t magic) {
    terminal_initialize();  

    if (MULTIBOOT_BOOTLOADER_MAGIC != magic) abort();

    hal_system_startup();
  
    printf("Anthrax OS booted by %s.\n", mbt->boot_loader_name);
    printf("Hello, kernel World!\n");
  
    printf("memory: %u\n", (mbt->mem_upper + mbt->mem_lower)/1024);
  
    multiboot_memory_map_t* entry = (multiboot_memory_map_t*) mbt->mmap_addr;
  
    while(entry < mbt->mmap_addr + mbt->mmap_length) {
	printf("mem entry: %llx, length %llx, type %x\n", entry->addr, entry->len, entry->type);
	entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    }

    while(1) {
	printf("read key: %c\n", getch());
    }
}

