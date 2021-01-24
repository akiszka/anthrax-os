#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H 1

#include <types.hpp>

// number of descriptors in the table
#define MAX_DESCRIPTORS 3

struct gdtr { // Special pointer for the GDT
	u16		m_limit; // size of gdt
	u32		m_base; // base address of gdt
} __attribute__ ((packed));

struct gdt_descriptor {
	u16 limitLo; // bits 0-15 of segment limit
	u16 baseLo; // bits 0-15 of base address
	u8  baseMid; // bits 16-23 of the base address
	u8  access; // access byte
	u8  limitHi_flags; // bits: LLLLFFFF; L - bits 16-20 of limit, F - flags
	                // this is terrible. too bad.
	u8  baseHi; // bits 24-32 of base address
} __attribute__ ((packed));

// ACCESS BYTE:
// 0 - Ac - accessed - just set to 0
// 1 - RW - readable/writeable - for code sectors - READABLE, for data sectors - WRITEABLE: data sectors always allow reading
// 2 - DC - direction/confirming -
       // direction for data sectors: 0 if grows up, 1 if grows down (offset > limit)
       // conforming for code sectors: 0 if code can only be executed by the ring set in Priv, 1 if code can be executed by a lower or equal link
// 3 - Ex - executable - 0 if data, 1 if can be executed
// 4 - S  - descriptor type - 1 for code or data, 0 for system (eg. TSS)
// 5 -Priv- privillege 1/2 + 0 = kernel, 3 = userland
// 6 -Priv- privillege 2/2 +
// 7 - Pr - present - must be 1

// FLAGS:
// 0 - 0  - just 0
// 1 - 0  - just 0
// 2 - Sz - size - 0 for 16-bit, 32 for 32-bit
// 3 - Gr - granularity - 0 for 1B block limit, 1 for 4KiB block limit

#define GDT_ACCESS_RW 0x2
#define GDT_ACCESS_DC 0x4
#define GDT_ACCESS_Ex 0x8
#define GDT_ACCESS_S  0x10
#define GDT_ACCESS_Userland 0x60
#define GDT_ACCESS_Present 0x80

#define GDT_FLAG_32_BIT 0x4
#define GDT_FLAG_4_KiB_LIMIT 0x8

// Returns the GDT selector. I is the index, ring is the ring number 0-3
// Selector - bits: 0-1 - ring; 2 - true for LDT false for GDT; 3-15 - index
#define gdt_get_selector(i, ring) ( ( (i & 0x1fff) << 3 ) | (ring & 0x3) )

void gdt_set_descriptor(u16 i, u32 base, u32 limit, u8 access, u8 flags);
void gdt_initialize();
void gdt_install();

#endif // ARCH_I386_GDT_H
