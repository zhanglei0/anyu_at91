#!/bin/bash
#qemu-system-arm -M integratorcp -kernel arch/arm/boot/zImage -m 32M -dtb arch/arm/boot/dts/vexpress-v2p-ca9.dtb -append "console=ttyAMA0" -nographic
qemu-system-arm -M integratorcp -kernel linux-at91-linux-2.6.39-at91/arch/arm/boot/zImage -m 32M -append "console=ttyAMA0" -nographic
