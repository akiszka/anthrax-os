#include <string.hpp>
#include <stdio.hpp>

#include "gdt.hpp"

struct gdt::descriptor _gdt [gdt::MAX_DESCRIPTORS];
struct gdt::gdtr       _gdtr;

void gdt::set_descriptor(u16 i, u32 base, u32 limit, u8 access, u8 flags) {
	if (i > MAX_DESCRIPTORS)
		return;

	// null out the descriptor
	memset ((void*)&_gdt[i], 0, sizeof (struct descriptor));

	// set limit and base addresses
	_gdt[i].baseLo	= base & 0xffff;
	_gdt[i].baseMid	= (base >> 16) & 0xff;
	_gdt[i].baseHi	= (base >> 24) & 0xff;
	_gdt[i].limitLo	= limit & 0xffff;

	// set flags and grandularity bytes
	_gdt[i].access = access;
	_gdt[i].limitHi_flags = (limit >> 16) & 0x0f;
	_gdt[i].limitHi_flags |= (flags & 0x0f) << 4;
}

void gdt::initialize() {
	_gdtr.m_limit = (sizeof (struct descriptor) * MAX_DESCRIPTORS)-1;
	_gdtr.m_base = (u32)(address) _gdt;

	debug_printf("GDTR limit: 0x%x base: 0x%x\n", _gdtr.m_limit, _gdtr.m_base);

	set_descriptor(0, 0, 0, 0, 0); // null descriptor
	set_descriptor(1, 0, 0xffffffff,
			   ACCESS_RW | ACCESS_DC | ACCESS_Ex | ACCESS_S | ACCESS_Present,
			   FLAG_32_BIT | FLAG_4_KiB_LIMIT); // code descriptor
	set_descriptor(2, 0, 0xffffffff,
			   ACCESS_RW | ACCESS_S | ACCESS_Present,
			   FLAG_4_KiB_LIMIT | FLAG_32_BIT); // data descriptor

	install();
}

__attribute__ ((optimize("O0"))) // I think I found a bug in GCC and this fixes it.
void gdt::install() {
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
	: "r"(get_selector(2, 0)),
	  "i"(get_selector(1, 0))
	);
}
