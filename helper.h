#ifndef HELPER_H
#define HELPER_H

#include "constants.h"
#include "game_phase.h"

#define BLINK 1
#define OFF 0

void display_column (uint8_t row_pattern, uint8_t current_column);

void move_cursor (uint8_t mode);

uint8_t button_pressed_p (void);

void display_4_seconds (void);

void display_3_seconds (uint8_t LED_MODE);

void show_winner (void);

void update_column (void);

void sink_ship (uint8_t temp_pos_y);

void display_blinking_cursor (void);

void display_ship_map (void);

void display_hit_map (void);

void display_empty_map (void);

void display_hit (void);

void display_miss (void);

void blink_led (void);

#endif
