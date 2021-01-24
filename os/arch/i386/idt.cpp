#include <stdio.hpp>
#include <string.hpp>
#include <types.hpp>
#include <kernel/tty.hpp>

#include "cpu.hpp"
#include "idt.hpp"

struct idt::descriptor    _idt [idt::MAX_INTERRUPTS];
struct idt::idtr              _idtr;

__attribute__ ((interrupt)) void default_ir(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_divide_by_zero(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_single_step(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_non_maskable_interrupt(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_breakpoint(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_overflow(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_bounds_check(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_undefined_opcode(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_no_coprocessor(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_double_fault(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_coprocessor_segment_overrun(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_invalid_tss(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_segment_not_present(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_stack_segment_overrun(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_gpf(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_page_fault(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_unassigned(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_coprocessor_error(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_alignment_check(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void ir_machine_check(struct interrupt_frame* frame);

void idt::generate_interrupt(u8 n) {
    // above code modifies the 0 to int number to generate
    asm volatile (
	"movb %%al, genint+1;"
	"jmp genint;"
	"genint:"
	"int $0;"
	:
	: "a"(n));
}

int idt::set_descriptor(u16 i, u16 code_selector, u8 flags, IRQ_HANDLER handler) {
    if (i > MAX_INTERRUPTS) return -1;
    if (!handler) return -1;

    u32 base = (address) &(*handler);

    _idt[i].baseLo = base & 0xffff;
    _idt[i].baseHi = (base >> 16) & 0xffff;
    _idt[i].sel = code_selector;
    _idt[i].flags = flags;
    _idt[i].reserved = 0;

    return 0;
}

void idt::initialize(u16 code_selector) {
    _idtr.m_limit = sizeof (struct descriptor) * MAX_INTERRUPTS-1;
    _idtr.m_base = (u32)(address) _idt;

    memset((void*)_idt, 0, sizeof (struct descriptor) * MAX_INTERRUPTS-1);

    set_descriptor(0, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_divide_by_zero);
    set_descriptor(1, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_single_step);
    set_descriptor(2, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_non_maskable_interrupt);
    set_descriptor(3, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_breakpoint);
    set_descriptor(4, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_overflow);
    set_descriptor(5, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_bounds_check);
    set_descriptor(6, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_undefined_opcode);
    set_descriptor(7, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_no_coprocessor);
    set_descriptor(8, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_double_fault);
    set_descriptor(9, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_coprocessor_segment_overrun);
    set_descriptor(10, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_invalid_tss);
    set_descriptor(11, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_segment_not_present);
    set_descriptor(12, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_stack_segment_overrun);
    set_descriptor(13, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_gpf);
    set_descriptor(14, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_page_fault);
    set_descriptor(15, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_unassigned);
    set_descriptor(16, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_coprocessor_error);
    set_descriptor(17, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_alignment_check);
    set_descriptor(18, code_selector, FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   ir_machine_check);

    for(u16 i = 19; i < MAX_INTERRUPTS; ++i)
	set_descriptor(i, code_selector,
			   FLAG_TYPE_32B_INT | FLAG_PRESENT,
			   default_ir);

    install();
}

void idt::install() {
    asm volatile ("lidt _idtr");
}

void idt::pic_initialize() {
    out8(PIC_PRIMARY_REG_CMD_STAT,   PIC_ICW1_IC4 | PIC_ICW1_INIT); io_wait();
    out8(PIC_SECONDARY_REG_CMD_STAT, PIC_ICW1_IC4 | PIC_ICW1_INIT); io_wait();

    out8(PIC_PRIMARY_REG_DATA_MASK,   PIC_ICW2_PRIMARY); io_wait();
    out8(PIC_SECONDARY_REG_DATA_MASK, PIC_ICW2_SECONDARY); io_wait();

    out8(PIC_PRIMARY_REG_DATA_MASK,   PIC_ICW3_PRIMARY); io_wait();
    out8(PIC_SECONDARY_REG_DATA_MASK, PIC_ICW3_SECONDARY); io_wait();

    out8(PIC_PRIMARY_REG_DATA_MASK,   PIC_ICW4_uPM); io_wait();
    out8(PIC_SECONDARY_REG_DATA_MASK, PIC_ICW4_uPM); io_wait();
}

void idt::pic_end_interrupt(u8 number) {
    if (PIC1_IRQ_BASE >= number && number < PIC2_IRQ_BASE) {
	out8(PIC_PRIMARY_REG_CMD_STAT, PIC_OCW2_EOI);
    } else if (PIC2_IRQ_BASE >= number &&  number < PIC2_IRQ_BASE+7) {
	out8(PIC_PRIMARY_REG_CMD_STAT,   PIC_OCW2_EOI);
	out8(PIC_SECONDARY_REG_CMD_STAT, PIC_OCW2_EOI);
    }
}

__attribute__ ((interrupt))
void default_ir(struct interrupt_frame* frame) {
    debug_printf("Unhandled interrupt or exception.\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_divide_by_zero(struct interrupt_frame* frame) {
    debug_printf("[INT] Divide by zero!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_single_step(struct interrupt_frame* frame) {
    debug_printf("[INT] Single step!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_non_maskable_interrupt(struct interrupt_frame* frame) {
    debug_printf("[INT] Non maskable interrupt!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_breakpoint(struct interrupt_frame* frame) {
    debug_printf("[INT] Breakpoint!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_overflow(struct interrupt_frame* frame) {
    debug_printf("[INT] Overflow!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_bounds_check(struct interrupt_frame* frame) {
    debug_printf("[INT] Bounds check!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_undefined_opcode(struct interrupt_frame* frame) {
    debug_printf("[INT] Undefined OPcode!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_no_coprocessor(struct interrupt_frame* frame) {
    debug_printf("[INT] No coprocessor!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_double_fault(struct interrupt_frame* frame) {
    debug_printf("[INT] Double fault!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_coprocessor_segment_overrun(struct interrupt_frame* frame) {
    debug_printf("[INT] Coprocessor segment overrun!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_invalid_tss(struct interrupt_frame* frame) {
    debug_printf("[INT] Invalid TSS!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_segment_not_present(struct interrupt_frame* frame) {
    debug_printf("[INT] Segment not present!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_stack_segment_overrun(struct interrupt_frame* frame) {
    debug_printf("[INT] Stack segment overrun\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_gpf(struct interrupt_frame* frame) {
    debug_printf("[INT] General protection fault!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_page_fault(struct interrupt_frame* frame) {
    debug_printf("[INT] Page fault\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_unassigned(struct interrupt_frame* frame) {
    debug_printf("[INT] (unassigned)\n");
}
__attribute__ ((interrupt))
void ir_coprocessor_error(struct interrupt_frame* frame) {
    debug_printf("[INT] Coprocessor error!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_alignment_check(struct interrupt_frame* frame) {
    debug_printf("[INT] Alignment check!\n");
    for(;;);
}
__attribute__ ((interrupt))
void ir_machine_check(struct interrupt_frame* frame) {
    debug_printf("[INT] Machine check!\n");
    for(;;);
}
