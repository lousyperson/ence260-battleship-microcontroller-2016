#ifndef GAME_H
#define GAME_H

#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_p1.h"
#include "game_p2.h"
#include "pio.h"

uint8_t no_ships;
uint8_t pos_x;
uint8_t pos_y;
uint8_t prev_pos_x;
uint8_t player;
uint8_t map_view;
uint16_t time;
uint8_t my_ship_count;
uint8_t ur_ship_count;
uint8_t ship_map[5];
uint8_t cursor_map[5];
uint8_t hit_map[5];
uint8_t previous_col;
const pio_t rows[7];
const pio_t cols[5];


#endif
