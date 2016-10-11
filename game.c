#include "init.h"
#include "constants.h"
#include "helper.h"
#include "preparation.h"
#include "planning.h"
#include "game_phase.h"

int main (void)
{
    game_init();

    while (1) {
        planning_phase ();
        preparation_phase ();
        game_phase ();
        show_winner ();
    }

    return 0;
}
