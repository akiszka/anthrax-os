#include "cpu.h"

#include <stdio.h>
#include <device/pci.h>

uint16_t pci_config_read_word (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    /*
      31 - enable bit
      30-24 - reserved
      23-16 - bus number
      15-11 - device number
      10-8 - function number
      7-0 - register offset (bits 1:0 must be 0)
    */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
			 (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    /* write out the address */
    out32(PCI_CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    uint32_t read = in32(PCI_CONFIG_DATA);
    tmp = (read >> ((offset & 3) * 8)) & 0xffff;
    return tmp;
}

pci_info pci_get_info(uint8_t bus, uint8_t slot) {
    pci_info info;
    /* try and read the first configuration register. Since there are no */
    /* vendors that == 0xFFFF, it must be a non-existent device. */
    if ((info.vendorID = pci_config_read_word(bus, slot, 0, 0)) != 0xFFFF) {
	info.deviceID = pci_config_read_word(bus, slot, 0, 2);
	info.class = pci_config_read_word(bus, slot, 0, 11);
	info.subclass = pci_config_read_word(bus, slot, 0, 0xA);
	info.header_type = pci_config_read_word(bus, slot, 0, 0xE);
	//. . .
    }
    return info;
}

void pci_check_device(uint8_t bus, uint8_t device) {
    uint8_t function = 0;

    pci_info info = pci_get_info(bus, device);
    if(info.vendorID == 0xFFFF) return;        // Device doesn't exist

    pci_check_function(bus, device, function);
    debug_printf("Found PCI device %x:%x, class: %x, subclass: %x, header: %x, MF: %x\n", info.vendorID, info.deviceID, info.class, info.subclass, info.header_type & 0x7F, (info.header_type & 0x80) >> 7);

    if((info.header_type & 0x80) != 0) {
	// It is a multi-function device, so check remaining functions
	for(function = 1; function < 8; function++) {
	    pci_check_function(bus, device, function);
	}
    }
}

void pci_check_function(uint8_t bus, uint8_t device, uint8_t function) {
    (void) bus;
    (void) device;
    (void) function;
}

void pci_enumerate(void) {
    uint16_t bus;
    uint8_t device;

    for(bus = 0; bus < 256; bus++) {
	for(device = 0; device < 32; device++) {
	    pci_check_device(bus, device);
	}
    }
}
