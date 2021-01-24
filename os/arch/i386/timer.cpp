#include <types.hpp>

#include "idt.hpp"
#include "timer.hpp"
#include "cpu.hpp"

static u64 _pit_ticks=0;

__attribute__ ((interrupt)) void pit_ir_timer(struct interrupt_frame* frame);

void timer::initialize(u16 code_selector) {
    idt::set_descriptor(idt::PIC1_IRQ_TIMER, code_selector,
			idt::FLAG_TYPE_32B_INT | idt::FLAG_PRESENT,
			pit_ir_timer);

    set_counter(PIT_OCW_SC0, PIT_OCW_RATE_GEN, 100); // every 10 ms
}

void timer::set_counter(u8 counter, u8 mode, u32 freq) {
    u16 divisor = PIT_INPUT_FREQ / freq;

    u8 ocw = 0;
    ocw |= PIT_OCW_TYPE_BINARY; // set type to binary
    ocw |= PIT_OCW_MASK_MODE & mode; // set the mode
    ocw |= PIT_OCW_MASK_RL   & PIT_OCW_RL_LSB_MSB; // set lsb and msb one after another
    ocw |= PIT_OCW_MASK_SC   & counter; // select the counter

    out8(PIT_REG_CONTROL, ocw); // send the OCW

    // send the divisor in two parts
    if (counter == PIT_OCW_SC0) {
	out8(PIT_REG_COUNTER0, divisor & 0xff);
	out8(PIT_REG_COUNTER0, (divisor >> 8) & 0xff);
	_pit_ticks = 0; // when setting the system counter, also reset the tick count
    } else if (counter == PIT_OCW_SC1) {
	out8(PIT_REG_COUNTER1, divisor & 0xff);
	out8(PIT_REG_COUNTER1, (divisor >> 8) & 0xff);
    } else if (counter == PIT_OCW_SC2) {
	out8(PIT_REG_COUNTER2, divisor & 0xff);
	out8(PIT_REG_COUNTER2, (divisor >> 8) & 0xff);
    }
}

u64 timer::get_ticks() {
    return _pit_ticks;
}

__attribute__ ((interrupt))
void pit_ir_timer(struct interrupt_frame* frame) {
    _pit_ticks++;
    if (_pit_ticks % 4000 == 0) {
	//debug_printf("Running for (sec): %lld\n", _pit_ticks/100);
    }
    idt::pic_end_interrupt(idt::PIC1_IRQ_TIMER);
}
