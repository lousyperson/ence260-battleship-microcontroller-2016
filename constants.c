#include "preparation.h"
#include "planning.h"
#include "constants.h"
#include "helper.h"
#include "game_phase.h"
#include "pio.h"

uint8_t no_ships = 0;

uint8_t pos_x = 2;
uint8_t pos_y = 3;
uint8_t prev_pos_x = 0;
uint8_t player = 0;
uint8_t map_view = 0;

uint16_t time = 0;

uint8_t my_ship_count = 3;
uint8_t ur_ship_count = 3;

uint8_t ship_map[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0
};

uint8_t cursor_map[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0
};

uint8_t hit_map[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0
};

uint8_t previous_col = 4;

/** Define PIO pins driving LED matrix rows.  */
const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};
