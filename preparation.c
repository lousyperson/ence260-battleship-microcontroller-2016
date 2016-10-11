#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_phase.h"

void preparation_phase (void)
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
            ir_uart_putc(0x47);
            if (ir_uart_getc () == 0x55) {
                player = 1;
                break;
            }
        }

        if (ir_uart_read_ready_p ())
        {
            if (ir_uart_getc () == 0x47) {
                ir_uart_putc(0x55);
                player = 2;
                break;
            }
        }
    }

    if (player == 1) {
        tinygl_draw_message ("P1", tinygl_point (0,0), 2);
    } else if (player == 2) {
        tinygl_draw_message ("P2", tinygl_point (0,0), 2);
    }

    display_4_seconds ();
}
