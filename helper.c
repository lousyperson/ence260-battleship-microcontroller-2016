#include "constants.h"
#include "game_phase.h"
#include "init.h"

/**Used to display bitmap*/
void display_column (uint8_t row_pattern, uint8_t current_column)
{
    int i;
    for (i = 0; i < 5; i++) {
        pio_output_high (cols[i]);
    }
    pio_output_low (cols[current_column]);

    uint8_t current_row;
    for (current_row = 0; current_row < 7; current_row++) {
		if ((row_pattern >> current_row) & 1) {
			pio_output_low (rows[current_row]);
		}
		else {
			pio_output_high (rows[current_row]);
		}
	}
}

/**Used to limit cursor movement*/
void move_cursor (uint8_t mode)
{
    if (navswitch_push_event_p (NAVSWITCH_NORTH) && pos_y > (1 - mode)) {
        cursor_map[pos_x] = cursor_map[pos_x] >> 1;
        pos_y--;
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pos_y < (5 + mode)) {
        cursor_map[pos_x] = cursor_map[pos_x] << 1;
        pos_y++;
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST) && pos_x < 4) {
        prev_pos_x = cursor_map[pos_x];
        cursor_map[pos_x] &= 0;
        pos_x++;
        cursor_map[pos_x] = prev_pos_x;
    }

    if (navswitch_push_event_p (NAVSWITCH_WEST) && pos_x > 0) {
        prev_pos_x = cursor_map[pos_x];
        cursor_map[pos_x] &= 0;
        pos_x--;
        cursor_map[pos_x] = prev_pos_x;
    }
}

/**Check if the button (s1) is pressed*/
uint8_t button_pressed_p (void)
{
    return pio_input_get(BUTTON_PIO);
}

/**Displays tinygl text for 4 seconds*/
void display_4_seconds (void)
{
    time = 0;
    while (time < (PACER_RATE * 4)) {
        pacer_wait ();
        tinygl_update ();
        time++;
    }
    tinygl_clear ();
}

/**Displays tinygl text for 3 seconds*/
void display_3_seconds (void)
{
    time = 0;
    while (time < (PACER_RATE * 3)) {
        pacer_wait ();
        tinygl_update ();
        time++;
    }
    tinygl_clear ();
}

/**Function to show winner text infinitely*/
void show_winner (void)
{
    if (my_ship_count == 0) {
        tinygl_text ("LOSE...");
    } else if (ur_ship_count == 0) {
        tinygl_text ("WIN!!!");
    }

    while (1) {
        pacer_wait ();
        tinygl_update ();
    }
}

/**Set current column for display_column*/
void update_column (void)
{
    current_column++;

    if (current_column > (LEDMAT_COLS_NUM - 1)) {
        current_column = 0;
    }
}

/**Function to turn off led for ship hit*/
void sink_ship (uint8_t temp_pos_y)
{
    int i, count = 0;
    uint8_t found_zero = 0b0;
    for (i = 0; i < 7; i++) {
        if ((ship_map[pos_x] >> i) & 1) {
            count++;
        } else {
            found_zero |= (1 << i);
        }
    }

    if (count > 3) {
        if (found_zero == 0b1000) {
            if (temp_pos_y <= 2) {
                ship_map[pos_x] ^= 0b111;
            } else if ((temp_pos_y >= 4) && (temp_pos_y <= 6)) {
                ship_map[pos_x] ^= 0b1110000;
            }
        } else if (found_zero == 0b1) {
            if ((temp_pos_y >= 1) && (temp_pos_y <= 3)) {
                ship_map[pos_x] ^= 0b1110;
            } else if ((temp_pos_y >= 4) && (temp_pos_y <= 6)) {
                ship_map[pos_x] ^= 0b1110000;
            }
        } else if (found_zero == 0b1000000) {
            if (temp_pos_y <= 2) {
                ship_map[pos_x] ^= 0b111;
            } else if ((temp_pos_y >= 3) && (temp_pos_y <= 5)) {
                ship_map[pos_x] ^= 0b111000;
            }
        }
    } else {
        ship_map[pos_x] &= 0;
    }
}

/**Function to display cursor with blinking effect*/
void display_blinking_cursor (void)
{
    if (cursor_timer_count < (BLINK_RATE / 2)) {
        display_column (cursor_map[current_column], current_column);
        pacer_wait ();
    }
    cursor_timer_count = (cursor_timer_count + 1) % (BLINK_RATE);
}

/**Function to display ship map*/
void display_ship_map (void)
{
    display_column (ship_map[current_column], current_column);
    pacer_wait ();
}

/**Function to display hit map*/
void display_hit_map (void)
{
    display_column (hit_map[current_column], current_column);
    pacer_wait ();
}

/**Function to display empty map*/
void display_empty_map (void)
{
    display_column (empty_map[current_column], current_column);
    pacer_wait ();
}

/**Function to display "HIT!" for 3 seconds*/
void display_hit (void)
{
    tinygl_draw_message ("HIT!", tinygl_point(0,0), 1);
    display_3_seconds ();
}

/**Function to display "MISS!" for 3 seconds*/
void display_miss (void)
{
    tinygl_draw_message ("MISS!", tinygl_point(0,0), 1);
    display_3_seconds ();
}
