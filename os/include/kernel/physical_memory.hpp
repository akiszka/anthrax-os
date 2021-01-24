#ifndef ARCH_I386_PHYSICAL_MEMORY_H
#define ARCH_I386_PHYSICAL_MEMORY_H

#include <types.hpp>

#define PMMGR_BLOCK_SIZE 4096 // 4096 B = 4 kB = page size
#define PMMGR_BLOCKS_TOTAL 1024*1024 // the number of pages = the number of blocks

void pmmgr_initialize();
address pmmgr_block_alloc();
void pmmgr_block_free(address ptr);
void pmmgr_set_region_used(address base, size_t size);
void pmmgr_set_region_unused(address base, size_t size);
void pmmgr_complete_initialization();

#endif // ARCH_I386_PHYSICAL_MEMORY_H
