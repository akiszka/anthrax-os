#ifndef _DEVICE_PCI_H
#define _DEVICE_PCI_H

#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct PCI_INFO_t {
    uint16_t vendorID;
    uint16_t deviceID;
    uint8_t class;
    uint8_t subclass;
    uint8_t header_type; // bit 7 - multiple functions bit, bits 6:0 - header type, 0 - standard, 1 - PCI-to-PCI bridge, 2 - CardBus bridge
} pci_info;

uint16_t pci_config_read_word (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
pci_info pci_get_info(uint8_t bus, uint8_t slot);
void pci_check_device(uint8_t bus, uint8_t device);
void pci_check_function(uint8_t bus, uint8_t device, uint8_t function);
void pci_enumerate(void);

#endif // _DEVICE_PCI_H
