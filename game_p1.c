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
#define PACER_RATE 500

void game_phase_p1 (void)
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
        display_column (cursor_map[current_column], current_column);
        pacer_wait ();
        display_column (hit_map[current_column], current_column);

        current_column++;
        previous_col = current_column - 1;

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

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
                    // Display player number for 2 seconds
                    time = 0;
                    while (time < (PACER_RATE * 3)) {
                        pacer_wait ();
                        tinygl_update ();
                        timer_task ();
                    }
                    tinygl_clear ();
                    player = 2;
                    break;
                } else if (rcv_point == 'M') {
                    tinygl_draw_message ("MISS!", tinygl_point(0,0), 1);
                    // Display player number for 2 seconds
                    time = 0;
                    while (time < (PACER_RATE * 3)) {
                        pacer_wait ();
                        tinygl_update ();
                        timer_task ();
                    }
                    tinygl_clear ();
                    player = 2;
                    break;
                }
            }
        }
    }
}
