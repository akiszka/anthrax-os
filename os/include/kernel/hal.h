#ifndef _KERNEL_HAL_H
#define _KERNEL_HAL_H

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

typedef uintptr_t address;

void hal_system_startup(multiboot_info_t* mbt);
void hal_system_shutdown();

uint8_t getch();

#endif // _KERNEL_HAL_H
