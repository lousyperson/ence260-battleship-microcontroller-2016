#include "led.h"
#include "constants.h"
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pio.h"
#include "ir_uart.h"
#include "helper.h"

#define BUTTON_PIO PIO_DEFINE(PORT_D, 7)

/**Used in move_cursor to define border for cursor*/
#define ATTACKING 1

/**Attacking phase*/
void game_p1 (void)
{
    led_set (LED1, 1);
    cursor_map[0] = 0b0;
    cursor_map[1] = 0b0;
    cursor_map[2] = 0b1000;
    cursor_map[3] = 0b0;
    cursor_map[4] = 0b0;

    current_column = 0;
    pos_x = 2;
    pos_y = 3;

    blink_timer_count = 0;

    while (1) {
        navswitch_update();
        display_blinking_cursor ();
        display_hit_map ();
        display_empty_map ();

        update_column ();

        // Skip turn if button (S1) is pressed
        if (button_pressed_p ()) {
            player = 2;
            break;
        }

        move_cursor(ATTACKING);

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (!(cursor_map[pos_x] & hit_map[pos_x])) {
                uint8_t send_point = 0b0;
                send_point |= (pos_x << 4);
                send_point |= (pos_y << 1);
                send_point |= 1;  // Used for error checking or reduce random signal picked up
                ir_uart_putc(send_point);

                uint8_t rcv_point = 0b0;

                rcv_point = ir_uart_getc ();

                if (rcv_point == 'H') {
                    ur_ship_count--;
                    display_hit ();  // Displays "HIT!" for 3 seconds
                    player = 2;
                    hit_map[pos_x] |= cursor_map[pos_x];
                    break;
                } else if (rcv_point == 'M') {
                    display_miss ();  // Displays "MISS!" for 3 seconds
                    player = 2;
                    hit_map[pos_x] |= cursor_map[pos_x];
                    break;
                }
            }
        }
    }
}

/**Defending phase*/
void game_p2 (void)
{
    led_set (LED1, 0);
    current_column = 0;
    map_view = 0;

    while (1) {
        if (map_view == 0) {
            display_ship_map ();
        } else {
            display_hit_map ();
        }
        display_empty_map ();

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
                uint8_t i;
                pos_y = 1;
                for (i = 0; i < temp_pos_y; i++) {
                    pos_y = pos_y << 1;
                }
                if (ship_map[pos_x] & pos_y) {
                    ir_uart_putc('H');
                    my_ship_count--;
                    sink_ship (temp_pos_y);  // Turn off led for ship hit
                    display_hit ();  // Displays "HIT!" for 3 seconds
                    player = 1;
                    break;
                } else if ((pos_x < 5) && (temp_pos_y < 7)) {
                    ir_uart_putc('M');
                    display_miss ();  // Displays "MISS!" for 3 seconds
                    player = 1;
                    break;
                }
            }
        }
    }
}

/**Main game function to alternate turns until one of the players ship count = 0*/
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
