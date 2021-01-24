#include <types.hpp>

#ifndef ARCH_I386_CPU_H
#define ARCH_I386_CPU_H

void out8(u16 port, u8 val);
void out16(u16 port, u16 value);
void out32(u16 port, u32 value);

u8 in8(u16 port);
u16 in16(u16 port);
u32 in32(u16 port);

void io_wait(void);

u8 are_interrupts_enabled();

u64 rdtsc();

#endif // ARCH_I386_CPU_H
