#ifndef ARCH_I386_PHYSICAL_MEMORY_H
#define ARCH_I386_PHYSICAL_MEMORY_H

#include <types.hpp>

namespace pmmgr {
    constexpr u32 BLOCK_SIZE = 4096; // 4096 B = 4 kB = page size
    constexpr u32 BLOCKS_TOTAL = 1024*1024; // the number of pages = the number of blocks

    void initialize();
    void complete_initialization();

    address block_alloc();
    void block_free(address ptr);

    void set_region_used(address base, size_t size);
    void set_region_unused(address base, size_t size);

    void set_block_used(u32 block_number);
    void set_block_unused(u32 block_number);
    u8 test_block(u32 block_number);
    u32 find_free_block();
}

#endif // ARCH_I386_PHYSICAL_MEMORY_H
