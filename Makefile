# File:   Makefile
# Author: Zhi You Tan, Sebastian Powell
# Date:   Oct 2016
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -Iapis/drivers -Iapis/drivers/avr -Iapis/fonts -Iapis/utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out

# Compile: create object files from C source files.
game.o: game.c apis/drivers/avr/ir_uart.h apis/drivers/avr/system.h apis/drivers/display.h apis/drivers/navswitch.h apis/fonts/font5x7_1.h apis/utils/font.h apis/utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: apis/drivers/avr/ir_uart.c apis/drivers/avr/ir_uart.h apis/drivers/avr/pio.h apis/drivers/avr/system.h apis/drivers/avr/timer0.h apis/drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: apis/drivers/avr/pio.c apis/drivers/avr/pio.h apis/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: apis/drivers/avr/prescale.c apis/drivers/avr/prescale.h apis/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: apis/drivers/avr/system.c apis/drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: apis/drivers/avr/timer.c apis/drivers/avr/system.h apis/drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: apis/drivers/avr/timer0.c apis/drivers/avr/bits.h apis/drivers/avr/prescale.h apis/drivers/avr/system.h apis/drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: apis/drivers/avr/usart1.c apis/drivers/avr/system.h apis/drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: apis/drivers/display.c apis/drivers/avr/system.h apis/drivers/display.h apis/drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: apis/drivers/ledmat.c apis/drivers/avr/pio.h apis/drivers/avr/system.h apis/drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: apis/drivers/navswitch.c apis/drivers/avr/delay.h apis/drivers/avr/pio.h apis/drivers/avr/system.h apis/drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: apis/utils/font.c apis/drivers/avr/system.h apis/utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: apis/utils/pacer.c apis/drivers/avr/system.h apis/drivers/avr/timer.h apis/utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: apis/utils/tinygl.c apis/drivers/avr/system.h apis/drivers/display.h apis/utils/font.h apis/utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

# Link: create ELF output file from object files.
game.out: game.o ir_uart.o pio.o prescale.o system.o timer.o timer0.o usart1.o display.o ledmat.o navswitch.o font.o tinygl.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@

# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex

# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start
