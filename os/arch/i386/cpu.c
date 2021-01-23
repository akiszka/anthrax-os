#include <stdint.h>

#include "cpu.h"

void out8(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void out16(uint16_t port, uint16_t value) {
    asm volatile("outw %0, %1" ::"a"(value), "Nd"(port));
}

void out32(uint16_t port, uint32_t value) {
    asm volatile("outl %0, %1" ::"a"(value), "Nd"(port));
}

uint8_t in8(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

uint16_t in16(uint16_t port) {
    uint16_t value;
    asm volatile("inw %1, %0"
                 : "=a"(value)
                 : "Nd"(port));
    return value;
}

uint32_t in32(uint16_t port) {
    uint32_t value;
    asm volatile("inl %1, %0"
                 : "=a"(value)
                 : "Nd"(port));
    return value;
}

void io_wait(void) {
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
    /* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}

uint8_t are_interrupts_enabled() {
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

uint64_t rdtsc() { // read the number of clock cycles elapsed since last reset
    uint64_t ret;
    asm volatile ( "rdtsc" : "=A"(ret) );
    return ret;
}
