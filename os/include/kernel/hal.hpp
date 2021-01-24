#ifndef _KERNEL_HAL_H
#define _KERNEL_HAL_H

#include <types.hpp>

#include "multiboot.h"

void hal_system_startup(multiboot_info_t* mbt);
void hal_system_shutdown();

u8 getch();

#endif // _KERNEL_HAL_H
