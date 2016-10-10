#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_p1.h"
#include "game_p2.h"

void game_phase_p2 (void)
{
    led_set (LED1, 0);
    uint8_t current_column = 0;
    map_view = 0;

    while (1) {
        pacer_wait ();

        if (map_view == 0) {
            display_column (ship_map[current_column], current_column);
        } else {
            display_column (hit_map[current_column], current_column);
        }

        current_column++;
        previous_col = current_column - 1;

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

        // Toggle map_view
        if (button_pressed_p ())
        {
            map_view ^= 1;
        }

        if (ir_uart_read_ready_p ())
        {
            uint8_t received_point = 0, temp_pos_y;
            received_point = ir_uart_getc ();
            if (received_point & 1) {
                pos_x = (received_point & 0b111000) >> 4;
                temp_pos_y = (received_point & 0b111) >> 1;
                uint8_t i, pos_y = 1;
                for (i = 0; i < temp_pos_y; i++) {
                    pos_y = pos_y << 1;
                }
                if (ship_map[pos_x] & pos_y) {
                    ir_uart_putc('H');
                    my_ship_count--;
                    ship_map[pos_x] &= 0;
                    tinygl_draw_message ("HIT!", tinygl_point(0,0), 1);
                    // Display player number for 2 seconds
                    time = 0;
                    while (time < (PACER_RATE * 3)) {
                        pacer_wait ();
                        tinygl_update ();
                        timer_task ();
                    }
                    tinygl_clear ();
                    player = 1;
                    break;
                } else {
                    ir_uart_putc('M');
                    tinygl_draw_message ("MISS!", tinygl_point(0,0), 1);
                    // Display player number for 2 seconds
                    time = 0;
                    while (time < (PACER_RATE * 3)) {
                        pacer_wait ();
                        tinygl_update ();
                        timer_task ();
                    }
                    tinygl_clear ();
                    player = 1;
                    break;
                }
            }
        }
    }
}
