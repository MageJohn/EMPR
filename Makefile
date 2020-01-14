#!/usr/bin/make -f

# Ensure consistent environment
SHELL := /bin/bash
PATH := /opt/york/cs/net/bin:$(PATH)

# Path to the GCC toolbox
PKG=/opt/york/cs/net

# Specify the commands needed from the tool chain, CC is the C-compiler,
# OBJCOPY converts the resulting program binary into a format we can load
# into the MBED board 
ARCH=arm-none-eabi
CC=$(ARCH)-gcc
OBJCOPY=$(ARCH)-objcopy

# Due to a change in 2012 Linux, we now need to set a user name variable in the
# Makefile so the output path the make install users is right, this is done as
# follows
# USER:=$(shell whoami)

SOURCERY=$(PKG)/sourcery-g++-lite-arm-eabi-2010.09.51-i686-1
GNU_VERSION=4.5.0
THUMB2GNULIB=$(SOURCERY)/$(ARCH)/lib/$(GNU_VERSION)/thumb2
THUMB2GNULIB2=$(SOURCERY)/$(ARCH)/lib/thumb2

# "Cortex Microcontroller Software Interface Standard" Startup files, also the
# flags passed to the C compiler, and linker
CMSIS=$(PKG)/lpc1700-cmsis-lite-2011.01.26-i686-1
CMSISINCLUDES=-I$(CMSIS)/include
CMSISFL=$(CMSIS)/lib/core_cm3.o \
	$(CMSIS)/lib/system_LPC17xx.o \
	$(CMSIS)/lib/startup_LPC17xx.o
LDSCRIPT = $(CMSIS)/lib/ldscript_rom_gnu.ld

CFLAGS=-mcpu=cortex-m3  -mthumb  -Wall  -O0  -mapcs-frame  -D__thumb2__=1 \
  -msoft-float  -gdwarf-2  -mno-sched-prolog  -fno-hosted  -mtune=cortex-m3 \
  -march=armv7-m  -mfix-cortex-m3-ldrd   -ffunction-sections  -fdata-sections \
          -D__RAM_MODE__=0 $(CMSISINCLUDES) -I. -I./lib

LDFLAGS=$(CMSISFL) -static -mcpu=cortex-m3 -mthumb -mthumb-interwork \
	   -Wl,--start-group -L$(THUMB2GNULIB) -L$(THUMB2GNULIB2) \
           -lc -lg -lstdc++ -lsupc++  -lgcc -lm  -Wl,--end-group \
	   -Xlinker -Map -Xlinker bin/lpc1700.map -Xlinker -T $(LDSCRIPT) -L

LDFLAGS+=-L$(CMSIS)/lib -lDriversLPC17xxgnu

CFLAGS += -MMD

.PHONY: clean install default

ROOT_DIR := $(shell pwd)
BIN_DIR := $(ROOT_DIR)/bin
SRC_DIR := $(ROOT_DIR)/src
LIB_DIR := $(ROOT_DIR)/lib

# Commands handled by this makefile
default:
	@echo "No default. Call make with a program name to be installed"

mp1_demo: leds.o serial.o
	$(MAKE) $@.bin deps="$^"

adc_test: serial.o wait.o
	$(MAKE) $@.bin deps="$^"

calculator: serial.o ioboard.a
	$(MAKE) $@.bin deps="$^"

%.install:
	$(MAKE) install name=$@

%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $(BIN_DIR)/$@

static/%.o: $(LIB_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $(BIN_DIR)/$@

%.bin: %.o
	$(CC) -o $(BIN_DIR)/$* $(BIN_DIR)/$^ $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BIN_DIR)/$* $(BIN_DIR)/$@
	rm $(BIN_DIR)/$*

# make clean - Clean out the source tree ready to re-build the project
clean:
	rm -f `find . | grep \~`
	rm -f *.swp *.o */*.o */*/*.o  *.log
	rm -f *.d */*.d *.srec */*.a bin/*.map
	rm -f *.elf *.wrn bin/* log *.hex

# make install - Installs the resulting binary file to the MBED board, remember
# to sync the file systems, so the copy finishes
# need to know user to install into correct directory
USER:=$(shell whoami)

install:
	@echo "Copying " $(BIN_DIR)/$(name) "to the MBED file system"
	cp $(BIN_DIR)/$(name).bin /media/$(USER)/MBED &
	sync
	@echo "Now press the reset button on all MBED file systems"

-include $(SRC_FILES:.h=.d)
-include $(LIB_FILES:.h=.d)
-include $($(wildcard $(LIB_DIR)/*/*.h):.h=.d)

.SECONDEXPANSION:
%.a: $$(patsubst $(LIB_DIR)/\%.c,$(BIN_DIR)/static/\%.o,$$(wildcard $(LIB_DIR)/%/*.c))
	ar rcs $(BIN_DIR)/static/$@ $^
