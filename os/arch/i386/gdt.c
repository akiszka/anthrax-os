#include <string.h>
#include <stdio.h>

#include "gdt.h"

struct gdt_descriptor _gdt [MAX_DESCRIPTORS];
struct gdtr           _gdtr;

void gdt_set_descriptor(uint16_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	if (i > MAX_DESCRIPTORS)
		return;
 
	// null out the descriptor
	memset ((void*)&_gdt[i], 0, sizeof (struct gdt_descriptor));
 
	// set limit and base addresses
	_gdt[i].baseLo	= base & 0xffff;
	_gdt[i].baseMid	= (base >> 16) & 0xff;
	_gdt[i].baseHi	= (base >> 24) & 0xff;
	_gdt[i].limitLo	= limit & 0xffff;
 
	// set flags and grandularity bytes
	_gdt[i].access = access;
	_gdt[i].limitHi_flags = (limit >> 16) & 0x0f;
	_gdt[i].limitHi_flags |= (flags & 0x0f) << 4;

	debug_printf("Setting descriptotr %u: base: %u limit: %u access: %u flags: %u\n", i, base, limit, access, flags);
	debug_printf("Result: %u %u %u %u %u %u\n", _gdt[i].limitLo, _gdt[i].baseLo, _gdt[i].baseMid, _gdt[i].access, _gdt[i].limitHi_flags, _gdt[i].baseHi);
}

void gdt_initialize() {
	_gdtr.m_limit = (sizeof (struct gdt_descriptor) * MAX_DESCRIPTORS)-1;
	_gdtr.m_base = (uint32_t) _gdt;

	debug_printf("GDTR limit: 0x%x base: 0x%x\n", _gdtr.m_limit, _gdtr.m_base);
	
	gdt_set_descriptor(0, 0, 0, 0, 0); // null descriptor
	gdt_set_descriptor(1, 0, 0xffffffff,
			   GDT_ACCESS_RW | GDT_ACCESS_DC | GDT_ACCESS_Ex | GDT_ACCESS_S | GDT_ACCESS_Present,
			   GDT_FLAG_32_BIT | GDT_FLAG_4_KiB_LIMIT); // code descriptor
	gdt_set_descriptor(2, 0, 0xffffffff,
			   GDT_ACCESS_RW | GDT_ACCESS_S | GDT_ACCESS_Present,
			   GDT_FLAG_4_KiB_LIMIT | GDT_FLAG_32_BIT); // data descriptor

	gdt_install();
}

__attribute__ ((optimize("O0"))) // I think I found a bug in GCC and this fixes it.
void gdt_install() {
    asm volatile ( 
	"lgdt _gdtr;" // load the GDT
	"mov %0, %%ds;" // load data segments
	"mov %0, %%es;"
	"mov %0, %%fs;"
	"mov %0, %%gs;"
	"mov %0, %%ss;"
	"ljmp %1, $reload_cs;" // load code segment
	"reload_cs:"
	:
	: "r"(gdt_get_selector(2, 0)),
	  "i"(gdt_get_selector(1, 0))
	);
}
