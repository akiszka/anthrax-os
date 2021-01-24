#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>

#include "cpu.h"
#include "idt.h"

void idt_install();

__attribute__ ((interrupt)) void idt_default_ir(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_divide_by_zero(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_single_step(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_non_maskable_interrupt(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_breakpoint(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_overflow(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_bounds_check(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_undefined_opcode(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_no_coprocessor(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_double_fault(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_coprocessor_segment_overrun(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_invalid_tss(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_segment_not_present(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_stack_segment_overrun(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_gpf(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_page_fault(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_unassigned(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_coprocessor_error(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_alignment_check(struct interrupt_frame* frame);
__attribute__ ((interrupt)) void idt_ir_machine_check(struct interrupt_frame* frame);

struct idt_descriptor    _idt [MAX_INTERRUPTS];
struct idtr              _idtr;

void idt_generate_interrupt(uint8_t n) {
    // above code modifies the 0 to int number to generate
    asm volatile (
	"movb %%al, genint+1;"
	"jmp genint;"
	"genint:"
	"int $0;"
	:
	: "a"(n));
}

int idt_set_descriptor(uint16_t i, uint16_t code_selector, uint8_t flags, IDT_IRQ_HANDLER handler) {
    if (i > MAX_INTERRUPTS) return -1;
    if (!handler) return -1;

    uint32_t base = (uint32_t)&(*handler);

    _idt[i].baseLo = base & 0xffff;
    _idt[i].baseHi = (base >> 16) & 0xffff;
    _idt[i].sel = code_selector;
    _idt[i].flags = flags;
    _idt[i].reserved = 0;

    return 0;
}

void idt_initialize(uint16_t code_selector) {
    _idtr.m_limit = sizeof (struct idt_descriptor) * MAX_INTERRUPTS-1;
    _idtr.m_base = (uint32_t)_idt;

    memset((void*)_idt, 0, sizeof (struct idt_descriptor) * MAX_INTERRUPTS-1);

    idt_set_descriptor(0, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_divide_by_zero);
    idt_set_descriptor(1, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_single_step);
    idt_set_descriptor(2, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_non_maskable_interrupt);
    idt_set_descriptor(3, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_breakpoint);
    idt_set_descriptor(4, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_overflow);
    idt_set_descriptor(5, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_bounds_check);
    idt_set_descriptor(6, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_undefined_opcode);
    idt_set_descriptor(7, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_no_coprocessor);
    idt_set_descriptor(8, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_double_fault);
    idt_set_descriptor(9, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_coprocessor_segment_overrun);
    idt_set_descriptor(10, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_invalid_tss);
    idt_set_descriptor(11, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_segment_not_present);
    idt_set_descriptor(12, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_stack_segment_overrun);
    idt_set_descriptor(13, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_gpf);
    idt_set_descriptor(14, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_page_fault);
    idt_set_descriptor(15, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_unassigned);
    idt_set_descriptor(16, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_coprocessor_error);
    idt_set_descriptor(17, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_alignment_check);
    idt_set_descriptor(18, code_selector, IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_ir_machine_check);

    for(uint16_t i = 19; i < MAX_INTERRUPTS; ++i)
	idt_set_descriptor(i, code_selector,
			   IDT_FLAG_TYPE_32B_INT | IDT_FLAG_PRESENT,
			   idt_default_ir);

    idt_install();
}

void idt_install() {
    asm volatile ("lidt _idtr");
}

void pic_initialize() {
    out8(PIC_PRIMARY_REG_CMD_STAT,   PIC_ICW1_IC4 | PIC_ICW1_INIT); io_wait();
    out8(PIC_SECONDARY_REG_CMD_STAT, PIC_ICW1_IC4 | PIC_ICW1_INIT); io_wait();

    out8(PIC_PRIMARY_REG_DATA_MASK,   PIC_ICW2_PRIMARY); io_wait();
    out8(PIC_SECONDARY_REG_DATA_MASK, PIC_ICW2_SECONDARY); io_wait();

    out8(PIC_PRIMARY_REG_DATA_MASK,   PIC_ICW3_PRIMARY); io_wait();
    out8(PIC_SECONDARY_REG_DATA_MASK, PIC_ICW3_SECONDARY); io_wait();

    out8(PIC_PRIMARY_REG_DATA_MASK,   PIC_ICW4_uPM); io_wait();
    out8(PIC_SECONDARY_REG_DATA_MASK, PIC_ICW4_uPM); io_wait();
}

void pic_end_interrupt(uint8_t number) {
    if (PIC1_IRQ_BASE >= number && number < PIC2_IRQ_BASE) {
	out8(PIC_PRIMARY_REG_CMD_STAT, PIC_OCW2_EOI);
    } else if (PIC2_IRQ_BASE >= number &&  number < PIC2_IRQ_BASE+7) {
	out8(PIC_PRIMARY_REG_CMD_STAT,   PIC_OCW2_EOI);
	out8(PIC_SECONDARY_REG_CMD_STAT, PIC_OCW2_EOI);
    }
}

__attribute__ ((interrupt))
void idt_default_ir(struct interrupt_frame* frame) {
    debug_printf("Unhandled interrupt or exception.\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_divide_by_zero(struct interrupt_frame* frame) {
    debug_printf("[INT] Divide by zero!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_single_step(struct interrupt_frame* frame) {
    debug_printf("[INT] Single step!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_non_maskable_interrupt(struct interrupt_frame* frame) {
    debug_printf("[INT] Non maskable interrupt!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_breakpoint(struct interrupt_frame* frame) {
    debug_printf("[INT] Breakpoint!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_overflow(struct interrupt_frame* frame) {
    debug_printf("[INT] Overflow!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_bounds_check(struct interrupt_frame* frame) {
    debug_printf("[INT] Bounds check!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_undefined_opcode(struct interrupt_frame* frame) {
    debug_printf("[INT] Undefined OPcode!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_no_coprocessor(struct interrupt_frame* frame) {
    debug_printf("[INT] No coprocessor!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_double_fault(struct interrupt_frame* frame) {
    debug_printf("[INT] Double fault!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_coprocessor_segment_overrun(struct interrupt_frame* frame) {
    debug_printf("[INT] Coprocessor segment overrun!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_invalid_tss(struct interrupt_frame* frame) {
    debug_printf("[INT] Invalid TSS!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_segment_not_present(struct interrupt_frame* frame) {
    debug_printf("[INT] Segment not present!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_stack_segment_overrun(struct interrupt_frame* frame) {
    debug_printf("[INT] Stack segment overrun\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_gpf(struct interrupt_frame* frame) {
    debug_printf("[INT] General protection fault!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_page_fault(struct interrupt_frame* frame) {
    debug_printf("[INT] Page fault\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_unassigned(struct interrupt_frame* frame) {
    debug_printf("[INT] (unassigned)\n");
}
__attribute__ ((interrupt))
void idt_ir_coprocessor_error(struct interrupt_frame* frame) {
    debug_printf("[INT] Coprocessor error!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_alignment_check(struct interrupt_frame* frame) {
    debug_printf("[INT] Alignment check!\n");
    for(;;);
}
__attribute__ ((interrupt))
void idt_ir_machine_check(struct interrupt_frame* frame) {
    debug_printf("[INT] Machine check!\n");
    for(;;);
}
