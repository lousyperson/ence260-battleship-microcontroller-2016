#include "init.h"
#include "constants.h"
#include "helper.h"
#include "preparation.h"
#include "planning.h"
#include "game_phase.h"

/** Main Program */
int main (void)
{
    game_init();

    tinygl_text ("Welcome to BATTLESHIP");
    while (1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update();

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            tinygl_clear ();
            break;
        }
    }

    planning_phase ();
    preparation_phase ();
    game_phase ();
    show_winner ();

    return 0;
}
