#ifndef HELPER_H
#define HELPER_H

#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_phase.h"

void timer_task (void);

void display_column (uint8_t row_pattern, uint8_t current_column);

void move_cursor (void);

uint8_t button_pressed_p (void);

void display_4_seconds (void);

void display_3_seconds (void);

void show_winner (void);

#endif
