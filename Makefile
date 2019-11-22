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
          -D__RAM_MODE__=0 $(CMSISINCLUDES) -I. 

LDFLAGS=$(CMSISFL) -static -mcpu=cortex-m3 -mthumb -mthumb-interwork \
	   -Wl,--start-group -L$(THUMB2GNULIB) -L$(THUMB2GNULIB2) \
           -lc -lg -lstdc++ -lsupc++  -lgcc -lm  -Wl,--end-group \
	   -Xlinker -Map -Xlinker bin/lpc1700.map -Xlinker -T $(LDSCRIPT)

LDFLAGS+=-L$(CMSIS)/lib -lDriversLPC17xxgnu

BINFOLDER=bin

# Name of the binary being built
MP1_EXECNAME = mp1_demo
LEDS_EXECNAME = leds_demo
I2C_SCAN_EXECNAME = i2c_scanner
LCD_TEST_EXECNAME = lcd_test
KP_TEST_EXECNAME = keypad_test
CALCULATOR_EXECNAME = calculator
MP2_EXECNAME = mp2_demo
ADC_TEST_EXECNAME = adc_test
DAC_TEST_EXECNAME = dac_test
SIGNAL_COPY_EXECNAME = signal_copy

EXECNAME = $(SIGNAL_COPY_EXECNAME)

# Source files provided by the user to build the project

MP1_OBJ = mp1_demo.o leds.o serial.o
LEDS_OBJ = leds_demo.o leds.o
I2C_SCAN_OBJ = i2c_scanner.o serial.o ioboard_i2c.o
LCD_TEST_OBJ = lcd_test.o ioboard_i2c.o serial.o ioboard_lcd.o
KP_TEST_OBJ = keypad_test.o ioboard_i2c.o ioboard_lcd.o ioboard_keypad.o serial.o
CALCULATOR_OBJ = calculator.o ioboard_i2c.o serial.o ioboard_lcd.o ioboard_keypad.o 
MP2_OBJ = mp2_demo.o ioboard_lcd.o wait.o i2c_scanner.o serial.o ioboard_i2c.o ioboard_keypad.o
ADC_TEST_OBJ = serial.o wait.o adc_test.o
DAC_TEST_OBJ = wait.o dac_test.o
SIGNAL_COPY_OBJ = signal_copy.o leds.o

# Commands handled by this makefile
all: 	mp1 leds i2c_scanner
	@echo "Build finished"

mp1_demo: $(MP1_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(MP1_EXECNAME) $(MP1_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(MP1_EXECNAME) $(BINFOLDER)/$(MP1_EXECNAME).bin

leds: $(LEDS_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(LEDS_EXECNAME) $(LEDS_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(LEDS_EXECNAME) $(BINFOLDER)/$(LEDS_EXECNAME).bin
	
i2c_scanner: $(I2C_SCAN_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(I2C_SCAN_EXECNAME) $(I2C_SCAN_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(I2C_SCAN_EXECNAME) $(BINFOLDER)/$(I2C_SCAN_EXECNAME).bin

lcd_test: $(LCD_TEST_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(LCD_TEST_EXECNAME) $(LCD_TEST_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(LCD_TEST_EXECNAME) $(BINFOLDER)/$(LCD_TEST_EXECNAME).bin

keypad_test: $(KP_TEST_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(KP_TEST_EXECNAME) $(KP_TEST_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(KP_TEST_EXECNAME) $(BINFOLDER)/$(KP_TEST_EXECNAME).bin

calculator: $(CALCULATOR_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(CALCULATOR_EXECNAME) $(CALCULATOR_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(CALCULATOR_EXECNAME) $(BINFOLDER)/$(CALCULATOR_EXECNAME).bin
mp2_demo: $(MP2_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(MP2_EXECNAME) $(MP2_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(MP2_EXECNAME) $(BINFOLDER)/$(MP2_EXECNAME).bin

adc_test: $(ADC_TEST_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(ADC_TEST_EXECNAME) $(ADC_TEST_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(ADC_TEST_EXECNAME) $(BINFOLDER)/$(ADC_TEST_EXECNAME).bin

dac_test: $(DAC_TEST_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(DAC_TEST_EXECNAME) $(DAC_TEST_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(DAC_TEST_EXECNAME) $(BINFOLDER)/$(DAC_TEST_EXECNAME).bin

signal_copy: $(SIGNAL_COPY_OBJ)
	mkdir -p bin # prevent error "No such file or directory" during linking
	$(CC) -o $(BINFOLDER)/$(SIGNAL_COPY_EXECNAME) $(SIGNAL_COPY_OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(BINFOLDER)/$(SIGNAL_COPY_EXECNAME) $(BINFOLDER)/$(SIGNAL_COPY_EXECNAME).bin

# make clean - Clean out the source tree ready to re-build the project
clean:
	rm -f `find . | grep \~`
	rm -f *.swp *.o */*.o */*/*.o  *.log
	rm -f *.d */*.d *.srec */*.a bin/*.map
	rm -f *.elf *.wrn bin/*.bin log *.hex
	rm -f $(EXECNAME)

# make install - Installs the resulting binary file to the MBED board, remember
# to sync the file systems, so the copy finishes
# need to know user to install into correct directory
USER:=$(shell whoami)

install:
	@echo "Copying " $(BINFOLDER)/$(EXECNAME) "to the MBED file system"
	cp $(BINFOLDER)/$(EXECNAME).bin /media/$(USER)/MBED &
	sync
	@echo "Now press the reset button on all MBED file systems"
