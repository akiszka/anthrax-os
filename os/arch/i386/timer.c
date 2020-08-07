#include <stdint.h>

#include "gdt.h"
#include "idt.h"
#include "timer.h"

static uint64_t _pit_ticks=0;

__attribute__ ((interrupt)) void pit_ir_timer(struct interrupt_frame* frame);

void pit_initialize() {
    uint16_t code_selector = gdt_get_selector(1, 0);
    idt_set_descriptor(PIC1_IRQ_TIMER, code_selector,
		       IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
		       pit_ir_timer);

    pit_set_counter(PIT_OCW_SC0, PIT_OCW_RATE_GEN, 100); // every 10 ms
}

void pit_set_counter(uint8_t counter, uint8_t mode, uint32_t freq) {
    uint16_t divisor = PIT_INPUT_FREQ / freq;
    
    uint8_t ocw = 0;
    ocw |= PIT_OCW_TYPE_BINARY; // set type to binary
    ocw |= PIT_OCW_MASK_MODE & mode; // set the mode
    ocw |= PIT_OCW_MASK_RL   & PIT_OCW_RL_LSB_MSB; // set lsb and msb one after another
    ocw |= PIT_OCW_MASK_SC   & counter; // select the counter

    outb(PIT_REG_CONTROL, ocw); // send the OCW

    // send the divisor in two parts
    if (counter == PIT_OCW_SC0) {
	outb(PIT_REG_COUNTER0, divisor & 0xff);
	outb(PIT_REG_COUNTER0, (divisor >> 8) & 0xff);
	_pit_ticks = 0; // when setting the system counter, also reset the tick count
    } else if (counter == PIT_OCW_SC1) {
	outb(PIT_REG_COUNTER1, divisor & 0xff);
	outb(PIT_REG_COUNTER1, (divisor >> 8) & 0xff);
    } else if (counter == PIT_OCW_SC2) {
	outb(PIT_REG_COUNTER2, divisor & 0xff);
	outb(PIT_REG_COUNTER2, (divisor >> 8) & 0xff);
    }
}

uint64_t pit_get_ticks() {
    return _pit_ticks;
}

__attribute__ ((interrupt))
void pit_ir_timer(struct interrupt_frame* frame) {
    _pit_ticks++;
    if (_pit_ticks % 100 == 0) {
	debug_printf("sec: %lld\n", _pit_ticks/100);
    }
    outb(PIC_PRIMARY_REG_CMD_STAT, PIC_OCW2_EOI);
}