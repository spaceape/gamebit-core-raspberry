# root makefile
# system: arm-none-eabi
export SYSTEM=rp2040
export CC=/usr/bin/arm-none-eabi-gcc
export CPP=/usr/bin/arm-none-eabi-g++
export CPU=cortex-m0plus
export DPU=soft
export FPU=soft
export GPU=
export BUILD_TYPE=
export BUILD_DIR=

ifeq ($(BUILD_TYPE),)
  BUILD_TYPE=none
  OPTIM=-O0
else ifeq ($(BUILD_TYPE),debug)
  BUILD_TYPE=debug
  OPTIM=-O0 -g
else ifeq ($(BUILD_TYPE),optim)
  BUILD_TYPE=optim
  OPTIM=-O3
endif

ifeq ($(BUILD_DIR),)
  BUILD_DIR=$(BASE_DIR)/base/$(BUILD_TYPE)
endif

export SYSFLAGS=-mcpu=$(CPU) -mthumb -ffreestanding -nostdlib -fno-common -falign-functions=4

export INCLUDES=-I$(WORKSPACE)/include -I$(WORKSPACE)/sys -I$(PWD)
export DEFINES=

export CFLAGS=-std=c11 -fpic -fverbose-asm -Wall -Wextra -save-temps $(OPTIM)
export CCFLAGS=-std=c++20 -fpic -fverbose-asm -Wall -Wextra -save-temps $(OPTIM)
export ASFLAGS=
export LDFLAGS=$(OPTIM) -fpie -Wl,--unresolved-symbols=ignore-all
export VERSION=0.0.1

export objects=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(objects)

%.elf: $(objects)
	$(CC) $(SYSFLAGS) $(LDFLAGS) $< -o $@

%.o: %.c makefile
	$(CC) $(SYSFLAGS) $(CFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

%.o: %.cpp makefile
	$(CC) $(SYSFLAGS) $(CCFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

clean:
	rm -fv *.o *.s *i *.elf

.PHONY: all lib src clean
