#include "constants.h"
#include "helper.h"
#include "game_phase.h"

/**Used in move_cursor to define border for cursor*/
#define PLANNING 0

/**Planning phase function (Step 1)*/
void planning_phase (void)
{
    cursor_map[2] = 0b11100;
    current_column = 0;
    pos_x = 2;
    pos_y = 3;

    cursor_timer_count = 0;

    while (1 && no_ships < 3)
    {
        navswitch_update ();

        display_blinking_cursor ();
        display_ship_map ();
        display_empty_map ();

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
