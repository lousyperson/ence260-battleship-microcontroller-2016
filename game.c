#include "system.h"
#include "pacer.h"
#include "pio.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "task.h"
#include "led.h"
#include "../fonts/font5x7_1.h"
#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_p1.h"
#include "game_p2.h"

#define PACER_RATE 500
#define MESSAGE_RATE 20

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

    // Display player number for 3 seconds
    time = 0;
    while (time < (PACER_RATE * 4)) {
        pacer_wait ();
        tinygl_update ();
        timer_task ();
    }

    tinygl_clear ();

    while (1 && my_ship_count > 0 && ur_ship_count > 0) {
        if (player == 1) {
            game_phase_p1();
        } else if (player == 2) {
            game_phase_p2();
        }
    }

    if (my_ship_count == 0) {
        tinygl_text ("LOSE...");
    } else if (ur_ship_count == 0) {
        tinygl_text ("WIN!!!");
    }

    while (1) {
        pacer_wait ();
        tinygl_update ();
    }

    return 0;
}
