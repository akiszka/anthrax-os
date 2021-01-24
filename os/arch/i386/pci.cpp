#include "cpu.hpp"

#include <stdio.hpp>
#include <device/pci.hpp>

u16 pci::config_read_word (u8 bus, u8 slot, u8 func, u8 offset) {
    u32 address;
    u32 lbus  = (u32)bus;
    u32 lslot = (u32)slot;
    u32 lfunc = (u32)func;
    u16 tmp = 0;

    /*
      31 - enable bit
      30-24 - reserved
      23-16 - bus number
      15-11 - device number
      10-8 - function number
      7-0 - register offset (bits 1:0 must be 0)
    */
    address = (u32)((lbus << 16) | (lslot << 11) |
			 (lfunc << 8) | (offset & 0xfc) | ((u32)0x80000000));

    /* write out the address */
    out32(CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    u32 read = in32(CONFIG_DATA);
    tmp = (read >> ((offset & 3) * 8)) & 0xffff;
    return tmp;
}

pci::pci_info pci::get_info(u8 bus, u8 slot) {
    pci_info info;
    /* try and read the first configuration register. Since there are no */
    /* vendors that == 0xFFFF, it must be a non-existent device. */
    if ((info.vendorID = config_read_word(bus, slot, 0, 0)) != 0xFFFF) {
	info.deviceID = config_read_word(bus, slot, 0, 2);
	info.mainclass = config_read_word(bus, slot, 0, 11);
	info.subclass = config_read_word(bus, slot, 0, 0xA);
	info.header_type = config_read_word(bus, slot, 0, 0xE);
    }
    return info;
}

void pci::check_device(u8 bus, u8 device) {
    u8 function = 0;

    pci_info info = get_info(bus, device);
    if(info.vendorID == 0xFFFF) return;        // Device doesn't exist

    check_function(bus, device, function);
    debug_printf("Found PCI device %x:%x, class: %x, subclass: %x, header: %x, MF: %x\n", info.vendorID, info.deviceID, info.mainclass, info.subclass, info.header_type & 0x7F, (info.header_type & 0x80) >> 7);

    if((info.header_type & 0x80) != 0) {
	// It is a multi-function device, so check remaining functions
	for(function = 1; function < 8; function++) {
	    check_function(bus, device, function);
	}
    }
}

void pci::check_function(u8 bus, u8 device, u8 function) {
    (void) bus;
    (void) device;
    (void) function;
}

void pci::enumerate(void) {
    u16 bus;
    u8 device;

    for(bus = 0; bus < 256; bus++) {
	for(device = 0; device < 32; device++) {
	    check_device(bus, device);
	}
    }
}
