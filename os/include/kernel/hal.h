#ifndef _KERNEL_HAL_H
#define _KERNEL_HAL_H

#include <stdint.h>

void hal_system_startup();
void hal_system_shutdown();

uint8_t getch();

#endif // _KERNEL_HAL_H
