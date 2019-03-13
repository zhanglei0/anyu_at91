#! /bin/bash

source setenv.sh

if [ $1 == "kernel" ];then 
    cd linux-at91-linux-2.6.39-at91/
    make ARCH=arm
    cp -a arch/arm/boot/zImage ../anyu06/
    cd ../anyu06/
    ./combine at91bootstrap.bin u-boot.bin zImage
    cd ../
fi
if [ $1 == "uboot" ];then
    cd u-boot-at91/
    ./build.sh
    cp -a u-boot.bin ../anyu06/
    cd ../anyu06/
    ./combine at91bootstrap.bin u-boot.bin zImage
    cd ../
fi
if [ $1 == "bootsharp" ];then
    cd at91bootstrap
    ./build.sh
    cp  binaries/at91bootstrap.bin ../anyu06/
    cd ../anyu06/
    ./combine at91bootstrap.bin u-boot.bin zImage
    cd ../
fi
