#ifndef _DEVICE_PCI_H
#define _DEVICE_PCI_H

#include <types.hpp>

namespace pci {
    constexpr u16 CONFIG_ADDRESS = 0xCF8;
    constexpr u16 CONFIG_DATA = 0xCFC;

    typedef struct INFO_t {
	u16 vendorID;
	u16 deviceID;
	u8 mainclass;
	u8 subclass;
	u8 header_type; // bit 7 - multiple functions bit, bits 6:0 - header type, 0 - standard, 1 - PCI-to-PCI bridge, 2 - CardBus bridge
    } pci_info;

    u16 config_read_word (u8 bus, u8 slot, u8 func, u8 offset);
    pci_info get_info(u8 bus, u8 slot);
    void check_device(u8 bus, u8 device);
    void check_function(u8 bus, u8 device, u8 function);
    void enumerate(void);
}

#endif // _DEVICE_PCI_H
