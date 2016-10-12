# File:   Makefile
# Author: Zhi You Tan, Sebastian Powell
# Date:   Oct 2016
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm

# Default target.
all: game.out

# Compile: create object files from C source files.
game.o: game.c init.h preparation.h planning.h constants.h helper.h game_phase.h
	$(CC) -c $(CFLAGS) $< -o $@

# game.o: game.c ../../drivers/avr/ir_uart.h ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/navswitch.h ../../fonts/font5x7_1.h ../../utils/font.h ../../utils/tinygl.h ../../drivers/avr/pio.h ../../utils/pacer.h ../../drivers/led.h ../../utils/task.h game_phase.h helper.h planning.h preparation.h constants.h
# 	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: ../../drivers/led.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

game_phase.o: game_phase.c game_phase.h ../../drivers/led.h constants.h ../../drivers/avr/system.h ../../utils/pacer.h ../../drivers/navswitch.h ../../utils/tinygl.h ../../drivers/avr/pio.h ../../drivers/avr/ir_uart.h helper.h  planning.h preparation.h
	$(CC) -c $(CFLAGS) $< -o $@

helper.o: helper.c helper.h game_phase.h planning.h preparation.h constants.h init.h
	$(CC) -c $(CFLAGS) $< -o $@

planning.o: planning.c planning.h game_phase.h helper.h preparation.h constants.h
	$(CC) -c $(CFLAGS) $< -o $@

preparation.o: preparation.c preparation.h game_phase.h helper.h planning.h constants.h
	$(CC) -c $(CFLAGS) $< -o $@

constants.o: constants.c constants.h game_phase.h helper.h planning.h preparation.h ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@

init.o: init.c init.h ../../drivers/avr/system.h ../../drivers/navswitch.h ../../utils/pacer.h ../../drivers/avr/ir_uart.h ../../utils/tinygl.h ../../fonts/font5x7_1.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
game.out: game.o ir_uart.o pio.o prescale.o system.o timer.o timer0.o usart1.o display.o ledmat.o navswitch.o font.o tinygl.o pacer.o led.o task.o game_phase.o helper.o planning.o preparation.o constants.o init.o
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
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex;
	dfu-programmer atmega32u2 start
