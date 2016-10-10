#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_p1.h"
#include "game_p2.h"

void planning_phase (void)
{
    cursor_map[2] = 0b11100;
    uint8_t current_column = 0;
    pos_x = 2;
    pos_y = 3;
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

        move_cursor();

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (!(cursor_map[pos_x] & ship_map[pos_x])) {
                ship_map[pos_x] |= cursor_map[pos_x];
                no_ships++;
            }
        }
    }
}
