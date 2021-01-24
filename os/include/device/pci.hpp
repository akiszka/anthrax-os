#ifndef _DEVICE_PCI_H
#define _DEVICE_PCI_H

#include <types.hpp>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct PCI_INFO_t {
    u16 vendorID;
    u16 deviceID;
    u8 mainclass;
    u8 subclass;
    u8 header_type; // bit 7 - multiple functions bit, bits 6:0 - header type, 0 - standard, 1 - PCI-to-PCI bridge, 2 - CardBus bridge
} pci_info;

u16 pci_config_read_word (u8 bus, u8 slot, u8 func, u8 offset);
pci_info pci_get_info(u8 bus, u8 slot);
void pci_check_device(u8 bus, u8 device);
void pci_check_function(u8 bus, u8 device, u8 function);
void pci_enumerate(void);

#endif // _DEVICE_PCI_H
