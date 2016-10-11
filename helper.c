#include "constants.h"
#include "game_phase.h"

// From stopwatch1
void timer_task (void)
{
    time++;
}

void display_column (uint8_t row_pattern, uint8_t current_column)
{
    pio_output_high (cols[previous_col]);
    pio_output_low (cols[current_column]);

    uint8_t current_row;
    for (current_row = 0; current_row < 7; current_row++) {
		if ((row_pattern >> current_row) & 1)
		{
			pio_output_low (rows[current_row]);
		}
		else
		{
			pio_output_high (rows[current_row]);
		}
	}
}

void move_cursor (void) {
    if (navswitch_push_event_p (NAVSWITCH_NORTH) && pos_y > 1) {
        cursor_map[pos_x] = cursor_map[pos_x] >> 1;
        pos_y--;
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pos_y < 5) {
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

// From lab1-ex4 : Check if the [button] is pressed
uint8_t button_pressed_p (void)
{
    return pio_input_get(BUTTON_PIO);
}

void display_4_seconds (void)
{
    // Display player number for 4 seconds
    time = 0;
    while (time < (PACER_RATE * 4)) {
        pacer_wait ();
        tinygl_update ();
        timer_task ();
    }
    tinygl_clear ();
}

void display_3_seconds (void)
{
    // Display player number for 3 seconds
    time = 0;
    while (time < (PACER_RATE * 3)) {
        pacer_wait ();
        tinygl_update ();
        timer_task ();
    }
    tinygl_clear ();
}

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

void update_column (void)
{
    current_column++;
    // previous_col = current_column - 1;

    if (previous_col == 0) {
        previous_col = 4;
    } else {
        previous_col = current_column - 1;
    }

    if (current_column > (LEDMAT_COLS_NUM - 1)) {
        current_column = 0;
    }
}
