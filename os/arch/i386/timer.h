#ifndef ARCH_I386_TIMER_H
#define ARCH_I386_TIMER_H

// * MISC *
#define PIT_INPUT_FREQ 1193180

// * REGISTERS *
#define PIT_REG_COUNTER0 0x40 // system timer
#define PIT_REG_COUNTER1 0x41 // don't use this counter, BIOS can be using it for something
#define PIT_REG_COUNTER2 0x42 // pc speaker
#define PIT_REG_CONTROL  0x43 // OCW goes gere

// * OCW *
#define PIT_OCW_TYPE_BINARY 0x00 // binary
#define PIT_OCW_TYPE_BCD    0x01 // binary coded decimal

#define PIT_OCW_MASK_MODE        0x0E // bits 1-3: counter mode
#define PIT_OCW_INTERRUPT_COUNT  0x00
#define PIT_OCW_ONESHOT          0x02 
#define PIT_OCW_RATE_GEN         0x04 // use this for a system clock
#define PIT_OCW_SQUAREWAVE_GEN   0x06 // use this for a speaker
#define PIT_OCW_SOFTWARE_STROBE  0x08
#define PIT_OCW_HARDWARE_STROBE  0x0A

#define PIT_OCW_MASK_RL    0x30 // bits 4-5: read/load mode (we need to load 2 bytes, one byte at a time)
#define PIT_OCW_RL_LATCH   0x00 // latch counter value into internal control register
#define PIT_OCW_RL_LSB     0x10 // load least significant byte only
#define PIT_OCW_RL_MSB     0x20 // load most significant byte only
#define PIT_OCW_RL_LSB_MSB 0x30 // load LSB first and then MSB

#define PIT_OCW_MASK_SC 0xC0 // bits 6-7: select counter
#define PIT_OCW_SC0     0x00 // select counter 0
#define PIT_OCW_SC1     0x40 // select counter 1
#define PIT_OCW_SC2     0x80 // select counter 2

// * FUNCTIONS *
void pit_initialize(uint16_t code_selector);
void pit_set_counter(uint8_t counter, uint8_t mode, uint32_t freq);
uint64_t pit_get_ticks();

#endif // ARCH_I386_TIMER_H
