#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/multiboot.h>

void gdt_initialize();
void idt_initialize();
void idt_generate_interrupt(uint8_t n);

void kernel_main(multiboot_info_t* mbt, uint32_t magic) {
  terminal_initialize();  

  if (MULTIBOOT_BOOTLOADER_MAGIC != magic) abort();

  gdt_initialize();
  idt_initialize();

  idt_generate_interrupt(5);

  // TODO: create Hardware Abstraction Layer functions
  // TODO: create a debug_printf() function that only works if a debug macro is set
  
  /*printf("Anthrax OS booted by %s.\n", mbt->boot_loader_name);
  printf("Hello, kernel World!\n");
  
  printf("memory: %u\n", mbt->mem_upper + mbt->mem_lower);
  
  multiboot_memory_map_t* entry = (multiboot_memory_map_t*) mbt->mmap_addr;
  
  while(entry < mbt->mmap_addr + mbt->mmap_length) {
    printf("mem entry: %llx, length %llx, type %x\n", entry->addr, entry->len, entry->type);
    entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    }*/
}
