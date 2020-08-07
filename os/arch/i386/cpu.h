#include <stdint.h>

#ifndef ARCH_I386_CPU_H
#define ARCH_I386_CPU_H

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);
uint8_t are_interrupts_enabled();
uint64_t rdtsc();

#endif // ARCH_I386_CPU_H
