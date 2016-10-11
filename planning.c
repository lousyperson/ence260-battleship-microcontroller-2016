#include "constants.h"
#include "helper.h"
#include "game_phase.h"

#define PLANNING 0

void planning_phase (void)
{
    cursor_map[2] = 0b11100;
    current_column = 0;
    pos_x = 2;
    pos_y = 3;

    while (1 && no_ships < 3)
    {
        pacer_wait ();
        navswitch_update ();
        tinygl_update ();
        display_column (cursor_map[current_column], current_column);
        pacer_wait ();
        display_column (ship_map[current_column], current_column);

        update_column ();

        move_cursor (PLANNING);

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (!(cursor_map[pos_x] & ship_map[pos_x])) {
                ship_map[pos_x] |= cursor_map[pos_x];
                no_ships++;
            }
        }
    }
}
