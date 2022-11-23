# root makefile

# system: arm-none-eabi
# /usr/include/c++/10/arm-none-eabi
# /usr/arm-suse-linux-gnueabi/sys-root 
# pio toolchain: $(HOME)/.platformio/packages/toolchain-gccarmnoneeabi

export SYSTEM=stm32
export SYSROOT=/usr
export CC=$(SYSROOT)/bin/arm-none-eabi-gcc
export CPP=$(SYSROOT)/bin/arm-none-eabi-g++
export MCU=cortex-m0 
export DPU=soft
export FPU=soft
export GPU=
export BASE_DIR=$(PWD)
export DIST_DIR=$(PWD)/dist
export BUILD_TYPE=
export BUILD_DIR=

ifeq ($(BUILD_TYPE),)
  BUILD_TYPE=none
  OPTIM=-O0
else ifeq ($(BUILD_TYPE),debug)
  BUILD_TYPE=debug
  OPTIM=-g -O0
else ifeq ($(BUILD_TYPE),optim)
  BUILD_TYPE=optim
  OPTIM=-O3
endif

ifeq ($(BUILD_DIR),)
  BUILD_DIR=$(BASE_DIR)/base/$(BUILD_TYPE)
endif

export SYSFLAGS=-mcpu=$(MCU) -mthumb

export CFLAGS=\
 -fpic -Wall -Wextra -Wpedantic $(OPTIM)

export CCFLAGS=\
 -std=c++20 -fpic -Wall -Wextra -Wpedantic $(OPTIM)

export LDFLAGS=
 
export INCLUDES=\
 -I.

# export DEFINES=\
#  -DF_CPU=72000000L -DSTM32F1 -DSTM32F103xB -DGENERIC_F103CX

export ASFLAGS=
export LDFLAGS=$(OPTIM)
export VERSION=0.0.1

export objects=sumxy.o sumxyz.o sumextern.o\
 localdata.o globaldata.o\
 localcall.o

all: $(objects)

%.o: %.c makefile
	$(CC) $(SYSFLAGS) $(CFLAGS) $(INCLUDES) $(DEFS) -save-temps -c $< -o $@

sumextern.o: sumextern.c makefile
	$(CC) $(SYSFLAGS) $(CFLAGS) $(INCLUDES) $(DEFS) -mlong-calls -save-temps -c $< -o $@

%.o: %.cpp makefile
	$(CC) $(SYSFLAGS) -MMD -MP $(CCFLAGS) $(INCLUDES) $(DEFS) -save-temps -c $< -o $@

clean:
	rm -fv $(objects)

.PHONY: all lib src clean
