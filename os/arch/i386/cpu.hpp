#include <stdint.h>

#ifndef ARCH_I386_CPU_H
#define ARCH_I386_CPU_H

void out8(uint16_t port, uint8_t val);
void out16(uint16_t port, uint16_t value);
void out32(uint16_t port, uint32_t value);

uint8_t in8(uint16_t port);
uint16_t in16(uint16_t port);
uint32_t in32(uint16_t port);

void io_wait(void);

uint8_t are_interrupts_enabled();

uint64_t rdtsc();

#endif // ARCH_I386_CPU_H
