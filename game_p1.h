#ifndef GAME_P1_H
#define GAME_P1_H

#include "led.h"
#include "constants.h"
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pio.h"
#include "ir_uart.h"
#include "helper.h"

#define BUTTON_PIO PIO_DEFINE(PORT_D, 7)
#define PACER_RATE 500

void game_phase_p1 (void);

#endif
