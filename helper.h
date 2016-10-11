#ifndef HELPER_H
#define HELPER_H

#include "constants.h"
#include "game_phase.h"

void display_column (uint8_t row_pattern, uint8_t current_column);

void move_cursor (uint8_t mode);

uint8_t button_pressed_p (void);

void display_4_seconds (void);

void display_3_seconds (void);

void show_winner (void);

void update_column (void);

#endif
