# Anthrax OS
## General info
This is a simple operating system written mostly in C. The goal of Anthrax is to be a learning tool and nothing more. It's based on [Meaty Skeleton](https://wiki.osdev.org/Meaty_Skeleton) from the OSDev Wiki.
## Building
Building Anthrax requires the following:
* [the tup build system](http://gittup.org/tup/index.html)
* a cross-platform toolchain built according to the [tutorial](https://wiki.osdev.org/GCC_Cross-Compiler) at OSDev Wiki
* QEMU (only if you want to test it afterwards)
If you're building Anthrax for the first time, run:
$ tup init
After that, building is as simple as running:
$ tup
To test if Anthrax works, use QEMU:
$ qemu-system-i386 -kernel os/myos.kernel
