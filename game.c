#include "system.h"
#include "pacer.h"
#include "pio.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "task.h"
#include "led.h"
#include "../fonts/font5x7_1.h"

#define BUTTON_PIO PIO_DEFINE(PORT_D, 7)
#define PACER_RATE 500
#define MESSAGE_RATE 10

// // Ship1 position
// static tinygl_point_t ship1_left_index;
// static tinygl_point_t ship1_mid_index;
// static tinygl_point_t ship1_right_index;
// // Ship2 position
// static tinygl_point_t ship2_left_index;
// static tinygl_point_t ship2_mid_index;
// static tinygl_point_t ship2_right_index;
// // Ship3 position
// static tinygl_point_t ship3_left_index;
// static tinygl_point_t ship3_mid_index;
// static tinygl_point_t ship3_right_index;
// Number of ships
static uint8_t no_ships = 0;

static uint8_t pos_x = 2;
static uint8_t pos_y = 3;
static uint8_t prev_pos_x = 0;
static uint8_t player = 0;
static uint8_t map_view = 0;

static uint16_t time;

// static tinygl_point_t ships[] =
// {
//   ship1_left_index,
//   ship1_mid_index,
//   ship1_right_index,
//
//   ship2_left_index,
//   ship2_mid_index,
//   ship2_right_index,
//
//   ship3_left_index,
//   ship3_mid_index,
//   ship3_right_index
// }

static uint8_t my_ship_count = 3;
static uint8_t ur_ship_count = 3;
// static uint8_t p = 1;

static uint8_t ship_map[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0
};

static uint8_t cursor_map[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0
};

static uint8_t hit_map[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0
};

// From stopwatch1
static void timer_task (void)
{
    time++;
}

static uint8_t previous_col;

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

static void display_column (uint8_t row_pattern, uint8_t current_column)
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

// From lab1-ex4 : Check if the [button] is pressed
static uint8_t button_pressed_p (void)
{
    return pio_input_get(BUTTON_PIO);
}

static void planning_phase (void) // solve whole row bug when 3 in a row
{
    cursor_map[2] = 0b11100;
    uint8_t current_column = 0;
    while (1 && no_ships < 3)
    {
        pacer_wait ();
        navswitch_update();
        tinygl_update();
        display_column (cursor_map[current_column], current_column);
        pacer_wait ();
        display_column (ship_map[current_column], current_column);

        current_column++;
        previous_col = current_column - 1;

        if (previous_col < 0) {
            previous_col = 4;
        }

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && pos_y > 1) {
            cursor_map[pos_x] = cursor_map[pos_x] >> 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pos_y < 5) {
            cursor_map[pos_x] = cursor_map[pos_x] << 1;
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

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (!(cursor_map[pos_x] & ship_map[pos_x])) {
                ship_map[pos_x] |= cursor_map[pos_x];
                no_ships++;
            }
        }
    }
}

static void preparation_phase (void)
{
    uint8_t current_column = 0;
    led_set (LED1, 1);
    while (1) {
        pacer_wait ();
        navswitch_update();

        if (map_view == 0) {
            display_column (ship_map[current_column], current_column);
        } else {
            display_column (hit_map[current_column], current_column);
        }

        current_column++;
        previous_col = current_column - 1;

        if (previous_col < 0) {
            previous_col = 4;
        }

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

        // Toggle map_view
        if (button_pressed_p ())
        {
            map_view ^= 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH))
        {
            ir_uart_putc(0x1);
            if (ir_uart_getc () == 0x2) {
                player = 1;
                break;
            }
        }

        if (ir_uart_read_ready_p ())
        {
            if (ir_uart_getc () == 0x1) {
                ir_uart_putc(0x2);
                player = 2;
                break;
            }
        }
    }
}

static void game_phase_p1 (void)
{
    led_set (LED1, 1);
    cursor_map[0] = 0b0;
    cursor_map[1] = 0b0;
    cursor_map[2] = 0b1000;
    cursor_map[3] = 0b0;
    cursor_map[4] = 0b0;

    uint8_t current_column = 0;
    pos_x = 2;
    pos_y = 3;

    while (1) {
        pacer_wait ();
        navswitch_update();
        display_column (cursor_map[current_column], current_column);
        pacer_wait ();
        display_column (hit_map[current_column], current_column);

        current_column++;
        previous_col = current_column - 1;

        if (previous_col < 0) {
            previous_col = 4;
        }

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && pos_y > 0) {
            cursor_map[pos_x] = cursor_map[pos_x] >> 1;
            pos_y--;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pos_y < 6) {
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

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (!(cursor_map[pos_x] & hit_map[pos_x])) {
                hit_map[pos_x] |= cursor_map[pos_x];
                uint8_t send_point = 0b0;
                send_point |= (pos_x << 3);
                send_point |= pos_y;
                ir_uart_putc(send_point);
                // HIT!
                if (ir_uart_getc () == 0x1) {
                    ur_ship_count--;
                    tinygl_draw_message ("HIT!", tinygl_point(0,0), 1);
                    // Display player number for 5 seconds
                    time = 0;
                    while (time < (PACER_RATE * 5)) {
                        pacer_wait ();
                        tinygl_update ();
                        timer_task ();
                    }
                    player = 2;
                    break;
                } else if (ir_uart_getc () == 0x2) {
                    tinygl_draw_message ("MISS!", tinygl_point(0,0), 1);
                    // Display player number for 5 seconds
                    time = 0;
                    while (time < (PACER_RATE * 5)) {
                        pacer_wait ();
                        tinygl_update ();
                        timer_task ();
                    }
                    player = 2;
                    break;
                }
            }
        }
    }
}

static void game_phase_p2 (void)
{
    led_set (LED1, 0);
    uint8_t current_column = 0;
    while (1) {
        pacer_wait ();
        display_column (ship_map[current_column], current_column);

        current_column++;
        previous_col = current_column - 1;

        if (previous_col < 0) {
            previous_col = 4;
        }

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

        if (ir_uart_read_ready_p ())
        {
            uint8_t received_point, temp_pos_y;
            received_point = ir_uart_getc ();
            pos_x = (received_point & 0b111000) >> 3;
            temp_pos_y = received_point & 0b111;
            uint8_t i, pos_y = 1;
            for (i = 0; i < temp_pos_y; i++) {
                pos_y = pos_y << 1;
            }
            if (ship_map[pos_x] & pos_y) {
                ir_uart_putc(0x1);
                my_ship_count--;
                tinygl_draw_message ("HIT!", tinygl_point(0,0), 1);
                // Display player number for 5 seconds
                time = 0;
                while (time < (PACER_RATE * 5)) {
                    pacer_wait ();
                    tinygl_update ();
                    timer_task ();
                }
                player = 1;
                break;
            } else {
                ir_uart_putc(0x2);
                tinygl_draw_message ("MISS!", tinygl_point(0,0), 1);
                // Display player number for 5 seconds
                time = 0;
                while (time < (PACER_RATE * 5)) {
                    pacer_wait ();
                    tinygl_update ();
                    timer_task ();
                }
                player = 1;
                break;
            }
        }
    }
}

int main (void)
{
    system_init ();
    navswitch_init ();
    pacer_init (PACER_RATE);
    ir_uart_init ();

    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    planning_phase();
    preparation_phase();

    if (player == 1) {
        tinygl_draw_message ("P1", tinygl_point(0,0), 2);
    } else if (player == 2) {
        tinygl_draw_message ("P2", tinygl_point(0,0), 2);
    }

    // task_t tasks[] =
    // {
    //     {.func = tinygl_update, .period = TASK_RATE / PACER_RATE},
    //     {.func = game_phase_p1, .period = TASK_RATE / PACER_RATE},
    //     {.func = game_phase_p2, .period = TASK_RATE / PACER_RATE},
    // };
    //
    // task_schedule (tasks, ARRAY_SIZE (tasks));

    // TCNT1 = 0;
    // while (TCNT1 < 60000)
    // {
    //     // tinygl_text ("DONE");
    //     pacer_wait ();
    //     tinygl_update();
    //
    //     // if (player == 1) {
    //     //     game_phase_p1();
    //     // } else if (player == 2) {
    //     //     game_phase_p2();
    //     // }
    // }

    // Display player number for 7 seconds
    time = 0;
    while (time < (PACER_RATE * 7)) {
        pacer_wait ();
        tinygl_update ();
        timer_task ();
    }

    while (1 && my_ship_count > 0 && ur_ship_count > 0) {
        if (player == 1) {
            game_phase_p1();
        } else if (player == 2) {
            game_phase_p2();
        }
    }

    return 0;

    // while (1)
    // {
    //     if (p == 1)
    //     {
    //         tinygl_update ();
    //         navswitch_update ();
    //     }
    //     else
    //     {
    //
    //     }
    // }
}
