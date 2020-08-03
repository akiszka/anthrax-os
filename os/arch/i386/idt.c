#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>

#include "idt.h"
#include "gdt.h"

struct idt_descriptor    _idt [MAX_INTERRUPTS];
struct idtr              _idtr;

void idt_generate_interrupt(uint8_t n) {
	// above code modifies the 0 to int number to generate
	asm __volatile__(
		"movb %%al, genint+1;"
		"jmp genint;"
		"genint:"
		"int $0;"	
		:
		: "a"(n));
}

void idt_default_ir() {
	terminal_initialize();
	printf("Unhandled interrupt.");
	for(;;);
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

void idt_initialize() {
	_idtr.m_limit = sizeof (struct idt_descriptor) * MAX_INTERRUPTS-1;
	_idtr.m_base = (uint32_t)_idt;

	memset((void*)_idt, 0, sizeof (struct idt_descriptor) * MAX_INTERRUPTS-1);
	
	uint16_t code_selector = gdt_get_selector(1, 0);
	for(uint16_t i = 0; i < MAX_INTERRUPTS; ++i) {
		idt_set_descriptor(i, code_selector, IDT_FLAG_TYPE_32B_INT|IDT_FLAG_PRESENT, idt_default_ir);
	}
	
	idt_install();
}

void idt_install() {
	asm __volatile__("lidt _idtr");
}
