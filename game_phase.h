#ifndef GAME_PHASE_H
#define GAME_PHASE_H

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
#define ATTACKING 1

void game_p1 (void);

void game_p2 (void);

void game_phase (void);

#endif
