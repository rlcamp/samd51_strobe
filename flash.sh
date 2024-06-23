#!/bin/bash
set -euo pipefail

# if bossac is not in path, try to find it where arduino puts it
if command -v bossac; then BOSSAC=bossac;
elif [ -x ${HOME}/Downloads/BOSSA/bin/bossac ]; then
    BOSSAC=${HOME}/Downloads/BOSSA/bin/bossac
elif [ -d ${HOME}/Library/Arduino15/packages/adafruit/tools/ ]; then
    BOSSAC=$(find ${HOME}/Library/Arduino15/packages/adafruit/tools/bossac -type f -name bossac)
fi

# if a command line argument was given, assume it is the /dev/cu.whatever corresponding to the
# device as it appears when sitting in the uf2 bootloader
if [ $# -eq 2 ]; then
# if the device exists, attempt to reset it into the bootloader. if it does not, you'll have to
# double tap the reset button
    if [ -e $2 ]; then
        stty speed 1200 < $2
        sleep 1
    else
        printf '%s not present, double tap the reset button and retry\n' $2 >&2
        exit 1
    fi

    $BOSSAC -i -d --port=$2 -U -i --offset=0x4000 -w -v $1 -R
else
    printf "Usage: ./flash.sh file.bin /dev/cu.usbmodem14501\n"
fi
