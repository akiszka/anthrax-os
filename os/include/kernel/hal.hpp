#ifndef _KERNEL_HAL_H
#define _KERNEL_HAL_H

#include <types.hpp>

#include "multiboot.h"

namespace hal {
    void system_startup(multiboot_info_t* mbt);
    void system_shutdown();

    u8 getch();
}

#endif // _KERNEL_HAL_H
