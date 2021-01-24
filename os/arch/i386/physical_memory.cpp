#include <stdint.h>
#include <string.hpp>
#include <stdio.hpp>
#include <stdlib.hpp>
#include <types.hpp>
#include <kernel/physical_memory.hpp>

static u32* pmmgr_bitmap = NULL; // size is in blocks

extern int __kernel_end;
extern int __kernel_start;
const address kernel_start = (address) &__kernel_start;
const address kernel_end = (address) &__kernel_end;
address kernel_end_aligned;

void pmmgr::initialize() {
    kernel_end_aligned = kernel_end + (BLOCK_SIZE - kernel_end % BLOCK_SIZE);
    pmmgr_bitmap = (u32*) kernel_end_aligned;
    memset(pmmgr_bitmap, 0xFFFFFFFF, BLOCKS_TOTAL/8);
}

void pmmgr::set_block_used(u32 block_number) {
    pmmgr_bitmap[block_number / 32] |= 1 << (block_number % 32);
}

void pmmgr::set_block_unused(u32 block_number) {
    pmmgr_bitmap[block_number / 32] &= ~(1 << (block_number % 32));
}

u8 pmmgr::test_block(u32 block_number) {
    return pmmgr_bitmap[block_number / 32] & (1 << (block_number % 32));
}

u32 pmmgr::find_free_block() {
    for (u32 i = 0; i < BLOCKS_TOTAL/32; ++i) {
	if (pmmgr_bitmap[i] != 0xFFFFFFFF) {
	    for (u8 j = 0; j < 32; ++j) {
		if (test_block(32*i+j) == 0) {
		    return 32*i+j;
		}
	    }
	}
    }

    return 0;
}

address pmmgr::block_alloc() {
    u32 block = find_free_block();
    if (0 == block) {
	printf("No available memory!\n");
	abort();
    }

    set_block_used(block);

    address ptr = block * BLOCK_SIZE;
    printf("found free blk: %u %lu\n", block, ptr);
    return ptr;
}

void pmmgr::block_free(address ptr) {
    u32 block = ptr / BLOCK_SIZE;
    set_block_unused(block);
}

void pmmgr::set_region_used(address base, size_t size) {
    u32 aligned = base / BLOCK_SIZE;
    u32 blocks = size / BLOCK_SIZE;
    if (size % BLOCK_SIZE > 0)
	blocks += 1;

    debug_printf("region used: base %lu size %lu blksize %u\n", base, size, blocks);

    for (; blocks > 0; --blocks) {
	set_block_used(aligned++);
    }
}

void pmmgr::set_region_unused(address base, size_t size) {
    u32 aligned = base / BLOCK_SIZE;
    u32 blocks = size / BLOCK_SIZE;

    debug_printf("region UNused: base %lu size %lu blksize %u\n", base, size, blocks);

    for (; blocks > 0; --blocks) {
	set_block_unused(aligned++);
    }

    set_block_used(0); //first block is always set. this ensures 0 can't be allocated
}

void pmmgr::complete_initialization() { // this is used to set the kernel memory and bitmap as used
    size_t kernel_size = kernel_end_aligned - kernel_start;
    set_region_used(kernel_start, kernel_size);

    // each block takes 1 bit, block size is in bytes
    set_region_used((address)pmmgr_bitmap, BLOCKS_TOTAL/8);

    debug_printf("Kernel end (page-aligned): %lu Pmmgr data end: %lu\n", kernel_end_aligned, ((address)pmmgr_bitmap)+BLOCKS_TOTAL/8);
}
