#ifndef GAME_H
#define GAME_H

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
uint8_t previous_col;
uint8_t current_column;
uint8_t ship_map[5];
uint8_t cursor_map[5];
uint8_t hit_map[5];
uint8_t empty_map[5];
const pio_t rows[7];
const pio_t cols[5];


#endif
