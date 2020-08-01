#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/multiboot.h>

void kernel_main(multiboot_info_t* mbt, unsigned int magic) {
  terminal_initialize();  
  printf("Anthrax OS booted by %s.\n", mbt->boot_loader_name);
  printf("Hello, kernel World!\n");
  multiboot_memory_map_t* entry = mbt->mmap_addr;
  while(entry < mbt->mmap_addr + mbt->mmap_length) {
    simple_printf("mem entry: %llx, length %llx, type %x\n", entry->addr, entry->len, entry->type);
    entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
  }
}
