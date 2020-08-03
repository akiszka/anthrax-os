#include <stdint.h>

#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H

#define MAX_INTERRUPTS 256

struct interrupt_frame
{
	uint32_t ip;
	uint32_t cs;
	uint32_t flags;
	uint32_t sp;
	uint32_t ss;
};

typedef void (*IDT_IRQ_HANDLER)(struct interrupt_frame*);

struct idt_descriptor {
	uint16_t		baseLo; // bits 0-15 of ir address
	uint16_t		sel; // code selector from gdt
	uint8_t			reserved; // should be 0
	uint8_t			flags; // flags
	uint16_t		baseHi; // bits 16-32 of ir address
} __attribute__((packed)); 

// FLAGS:
// 0 - Type - Gate type 1/4 - 0x5 for 32-bit task, 0xE for 32-bit interrupt,
// 1 - Type - Gate type 2/4 - 0xF for 32-bit trap
// 2 - Type - Gate type 3/4 -
// 3 - Type - Gate type 4/4 -
// 4 - S    - Storage Segment - 0 if interrupt or trap gate (see above) 
// 5 - DPL  - Descriptor Privilege Level - Kernel ring number
// 6 - DPL  - Descriptor Privilege Level - 
// 7 - P    - Present - 1 if used

#define IDT_FLAG_TYPE_32B_TASK 0x5
#define IDT_FLAG_TYPE_32B_INT 0xE
#define IDT_FLAG_TYPE_32B_TRAP 0xF
#define IDT_FLAG_S_NOT_INT_OR_TRAP 0x10
#define IDT_FLAG_Userland 0x60
#define IDT_FLAG_PRESENT 0x80

struct idtr { // idt pointer
	uint16_t		m_limit; // idt size
	uint32_t		m_base; // idt base address
} __attribute__((packed));

void idt_generate_interrupt(uint8_t n);
__attribute__ ((interrupt))
void idt_default_ir(struct interrupt_frame* frame);

int idt_set_descriptor(uint16_t i, uint16_t code_selector, uint8_t flags, IDT_IRQ_HANDLER handler);
void idt_initialize();
void idt_install();

#endif // ARCH_I386_IDT_H
