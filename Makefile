# see STM32-base/startup/STM32F0xx/ for possible devices
DEVICE = STM32F030x4
FLASH  = 0x08000000

USE_ST_CMSIS = true

TOOLCHAIN_PATH = /usr/bin/

# Include the main makefile
include ./STM32-base/make/common.mk
