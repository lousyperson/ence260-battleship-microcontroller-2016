#include "system.h"
#include "navswitch.h"
#include "pacer.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
// #include "pio.h"
// #include "task.h"
// #include "led.h"
#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_phase.h"

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

    planning_phase ();
    preparation_phase ();
    game_phase ();
    show_winner ();

    return 0;
}
