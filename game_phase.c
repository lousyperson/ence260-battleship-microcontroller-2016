#include "led.h"
#include "constants.h"
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pio.h"
#include "ir_uart.h"
#include "helper.h"

#include "preparation.h"
#include "planning.h"

#define BUTTON_PIO PIO_DEFINE(PORT_D, 7)
#define PACER_RATE 500

void game_p1 (void)
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
        tinygl_clear ();
        tinygl_update ();
        display_column (cursor_map[current_column], current_column);
        pacer_wait ();
        display_column (hit_map[current_column], current_column);

        update_column ();

        if (button_pressed_p ()) {
            player = 2;
            break;
        }

        move_cursor();

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (!(cursor_map[pos_x] & hit_map[pos_x])) {
                uint8_t send_point = 0b0;
                send_point |= (pos_x << 4);
                send_point |= (pos_y << 1);
                send_point |= 1;
                ir_uart_putc(send_point);
                // HIT!
                uint8_t rcv_point = 0b0;
                rcv_point = ir_uart_getc ();
                hit_map[pos_x] |= cursor_map[pos_x];

                if (rcv_point == 'H') {
                    ur_ship_count--;
                    tinygl_draw_message ("HIT!", tinygl_point(0,0), 1);
                    // Display HIT! for 2 seconds
                    display_3_seconds ();
                    player = 2;
                    break;
                } else if (rcv_point == 'M') {
                    tinygl_draw_message ("MISS!", tinygl_point(0,0), 1);
                    // Display MISS! for 2 seconds
                    display_3_seconds ();
                    player = 2;
                    break;
                }
            }
        }
    }
}

void game_p2 (void)
{
    led_set (LED1, 0);
    current_column = 0;
    map_view = 0;

    while (1) {
        pacer_wait ();

        if (map_view == 0) {
            display_column (ship_map[current_column], current_column);
        } else {
            display_column (hit_map[current_column], current_column);
        }

        update_column ();

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
                pos_x = (received_point & 0b1110000) >> 4;
                temp_pos_y = (received_point & 0b1110) >> 1;
                uint8_t i, pos_y = 1;
                for (i = 0; i < temp_pos_y; i++) {
                    pos_y = pos_y << 1;
                }
                if (ship_map[pos_x] & pos_y) {
                    ir_uart_putc('H');
                    my_ship_count--;
                    ship_map[pos_x] &= 0;
                    tinygl_draw_message ("HIT!", tinygl_point(0,0), 1);
                    // Display HIT! for 3 seconds
                    display_3_seconds ();
                    player = 1;
                    break;
                } else {
                    ir_uart_putc('M');
                    tinygl_draw_message ("MISS!", tinygl_point(0,0), 1);
                    // Display MISS! for 3 seconds
                    display_3_seconds ();
                    player = 1;
                    break;
                }
            }
        }
    }
}

void game_phase (void)
{
    while (1 && my_ship_count > 0 && ur_ship_count > 0) {
        if (player == 1) {
            game_p1 ();
        } else if (player == 2) {
            game_p2 ();
        }
    }
}
