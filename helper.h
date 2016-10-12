#ifndef HELPER_H
#define HELPER_H

#include "constants.h"
#include "game_phase.h"

#define BLINK 1
#define OFF 0

/**Used to display bitmap*/
void display_column (uint8_t row_pattern, uint8_t current_column);

/**Used to limit cursor movement*/
void move_cursor (uint8_t mode);

/**Check if the button (s1) is pressed*/
uint8_t button_pressed_p (void);

/**Displays tinygl text for 4 seconds*/
void display_4_seconds (void);

/**Displays tinygl text for 3 seconds*/
void display_3_seconds (uint8_t LED_MODE);

/**Function to show winner text infinitely*/
void show_winner (void);

/**Set current column for display_column*/
void update_column (void);

/**Function to turn off led for ship hit*/
void sink_ship (uint8_t temp_pos_y);

/**Function to display cursor with blinking effect*/
void display_blinking_cursor (void);

/**Function to display ship map*/
void display_ship_map (void);

/**Function to display hit map*/
void display_hit_map (void);

/**Function to display empty map*/
void display_empty_map (void);

/**Function to display "HIT!" for 3 seconds*/
void display_hit (void);

/**Function to display "MISS!" for 3 seconds*/
void display_miss (void);

/**Function to blink led1*/
void blink_led (void);

#endif
