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

/**Used in move_cursor to define border for cursor*/
#define ATTACKING 1

/**Attacking phase*/
void game_p1 (void);

/**Defending phase*/
void game_p2 (void);

/**Main game function to alternate turns until one of the players ship count = 0*/
void game_phase (void);

#endif
