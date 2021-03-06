#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H

#include <types.hpp>

typedef void (*IRQ_HANDLER)(struct interrupt_frame*);

namespace idt {
    constexpr u16 MAX_INTERRUPTS = 256;

    struct interrupt_frame
    {
	u32 ip;
	u32 cs;
	u32 flags;
	u32 sp;
	u32 ss;
    };

    struct descriptor {
	u16		baseLo; // bits 0-15 of ir address
	u16		sel; // code selector from gdt
	u8          reserved; // should be 0
	u8		flags; // flags
	u16		baseHi; // bits 16-32 of ir address
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

    constexpr u8 FLAG_TYPE_32B_TASK = 0x5;
    constexpr u8 FLAG_TYPE_32B_INT = 0xE;
    constexpr u8 FLAG_TYPE_32B_TRAP = 0xF;
    constexpr u8 FLAG_S_NOT_INT_OR_TRAP = 0x10;
    constexpr u8 FLAG_Userland = 0x60;
    constexpr u8 FLAG_PRESENT = 0x80;

    struct idtr { // idt pointer
	u16 m_limit; // idt size
	u32 m_base; // idt base address
    } __attribute__((packed));

    constexpr u16 PIC_PRIMARY_REG_CMD_STAT = 0x20;
    constexpr u16 PIC_PRIMARY_REG_DATA_MASK = 0x21;

    constexpr u16 PIC_SECONDARY_REG_CMD_STAT = 0xA0;
    constexpr u16 PIC_SECONDARY_REG_DATA_MASK = 0xA1;

    constexpr u8 PIC_ICW1_IC4  = 0x01; // whether to expect ICW4
    constexpr u8 PIC_ICW1_SNGL = 0x02; // set if only one PIC in the system (NOT in x86)
    constexpr u8 PIC_ICW1_ADI  = 0x04; // ignored on x86, do not set
    constexpr u8 PIC_ICW1_LTIM = 0x08; // set for level triggered, unset for edge triggered
    constexpr u8 PIC_ICW1_INIT = 0x10; // set to initialize the PIC

    constexpr u8 PIC_ICW2_PRIMARY   = 0x20; // map ints 32-39 to PIC1
    constexpr u8 PIC_ICW2_SECONDARY = 0x28; // map ints 40-47 to PIC2

    constexpr u8 PIC_ICW3_PRIMARY   = 0x04; // IR line 2 for communication, it just is like that on x86
    constexpr u8 PIC_ICW3_SECONDARY = 0x02; // IR line 2 for communication, it just is like that on x86

    constexpr u8 PIC_ICW4_uPM  = 0x01; // set if x86 mode
    constexpr u8 PIC_ICW4_AEOI = 0x02; // PIC will automatically perform EOI on last int ack pulse
    constexpr u8 PIC_ICW4_MS   = 0x04; // only use if BUF is set, if this is set, selects buffer master
    constexpr u8 PIC_ICW4_BUF  = 0x08; // set for buffered mode
    constexpr u8 PIC_ICW4_SFNM = 0x10; // set for special fully nested mode, do not use on x86

    inline constexpr u16 PIC_OCW1_MASK(u8 irq) { // mask an interrupt
	return 1 << irq;
    }

/*
  OCW2 COMMANDS
  R Bit	SL Bit	EOI Bit	Description
  0	0	0	Rotate in Automatic EOI mode (CLEAR)
  0	0	1	Non specific EOI command
  0	1	0	No operation
  0	1	1	Specific EOI command
  1	0	0	Rotate in Automatic EOI mode (SET)
  1	0	1	Rotate on non specific EOI
  1	1	0	Set priority command
  1	1	1	Rotate on specific EOI
*/
    inline constexpr u8 PIC_OCW2_L(u8 level) { // int level upon which to react
	return level & 0x03;
    }

    constexpr u8 PIC_OCW2_EOI = 0x20; // end of interrupt signal
    constexpr u8 PIC_OCW2_SL  = 0x40; // selection
    constexpr u8 PIC_OCW2_R   = 0x80; // rotation

// The following  devices use PIC 1 to generate interrupts
    constexpr u8 PIC1_IRQ_BASE      = 32;
    constexpr u8 PIC1_IRQ_TIMER     = 0+32; // (the interrupts from PIC1 are mapped to 32-39)
    constexpr u8 PIC1_IRQ_KEYBOARD  = 1+32;
    constexpr u8 PIC1_IRQ_SERIAL2   = 3+32;
    constexpr u8 PIC1_IRQ_SERIAL1   = 4+32;
    constexpr u8 PIC1_IRQ_PARALLEL2 = 5+32;
    constexpr u8 PIC1_IRQ_DISKETTE  = 6+32;
    constexpr u8 PIC1_IRQ_PARALLEL1 = 7+32;

// The following devices use PIC 2 to generate interrupts
    constexpr u8 PIC2_IRQ_BASE       = 40;
    constexpr u8 PIC2_IRQ_CMOSTIMER  = 0+40; // (the interrupts from PIC2 are mapped to 40-47)
    constexpr u8 PIC2_IRQ_CGARETRACE = 1+40;
    constexpr u8 PIC2_IRQ_AUXILIARY  = 4+40;
    constexpr u8 PIC2_IRQ_FPU        = 5+40;
    constexpr u8 PIC2_IRQ_HDC        = 6+40;

    void generate_interrupt(u8 n);
    int set_descriptor(u16 i, u16 code_selector, u8 flags, IRQ_HANDLER handler);
    void initialize(u16 code_selector);
    void install();
    void pic_initialize();
    void pic_end_interrupt(u8 number);
}

#endif // ARCH_I386_IDT_H
