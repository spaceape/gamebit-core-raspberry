# root makefile
# system: arm-none-eabi
export SYSTEM=rp2040
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
  OPTIM=-O0 -g
else ifeq ($(BUILD_TYPE),optim)
  BUILD_TYPE=optim
  OPTIM=-O3
endif

ifeq ($(BUILD_DIR),)
  BUILD_DIR=$(BASE_DIR)/base/$(BUILD_TYPE)
endif

export SYSFLAGS=-mcpu=$(MCU) -mthumb -mno-long-calls -nostdlib -falign-functions=4 -falign-labels=4

export INCLUDES=-I.
export DEFINES=

export CFLAGS=-fpic -fverbose-asm -Wall -Wextra -save-temps $(OPTIM)
export CCFLAGS=-std=c++20 -fpic -fverbose-asm -Wall -Wextra -save-temps $(OPTIM)
export ASFLAGS=
export LDFLAGS=$(OPTIM) -fpie -Wl,--unresolved-symbols=ignore-all
export VERSION=0.0.1

export objects=game.o

all: $(objects)

%.elf: $(objects)
	$(CC) $(SYSFLAGS) $(LDFLAGS) $< -o $@

%.o: %.c %.h makefile
	$(CC) $(SYSFLAGS) $(CFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

%.o: %.cpp %.h makefile
	$(CC) $(SYSFLAGS) $(CCFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

clean:
	rm -fv *.o *.s *i *.elf

.PHONY: all lib src clean
