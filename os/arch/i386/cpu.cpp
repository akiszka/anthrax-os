#include <types.hpp>
#include "cpu.hpp"

void out8(u16 port, u8 val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void out16(u16 port, u16 value) {
    asm volatile("outw %0, %1" ::"a"(value), "Nd"(port));
}

void out32(u16 port, u32 value) {
    asm volatile("outl %0, %1" ::"a"(value), "Nd"(port));
}

u8 in8(u16 port) {
    u8 ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

u16 in16(u16 port) {
    u16 value;
    asm volatile("inw %1, %0"
                 : "=a"(value)
                 : "Nd"(port));
    return value;
}

u32 in32(u16 port) {
    u32 value;
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

u8 are_interrupts_enabled() {
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

u64 rdtsc() { // read the number of clock cycles elapsed since last reset
    u64 ret;
    asm volatile ( "rdtsc" : "=A"(ret) );
    return ret;
}
