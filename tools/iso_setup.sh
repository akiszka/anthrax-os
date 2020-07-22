#!/bin/sh

cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Anthrax" {
	multiboot /boot/anthrax.kernel
}
EOF
