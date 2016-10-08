#include "system.h"
// #include "pacer.h"
#include "navswitch.h"
// #include "ir_uart.h"
#include "tinygl.h"
// #include "task.h"
#include "../fonts/font5x7_1.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10

// Ship1 position
static tinygl_point_t ship1_left_index;
static tinygl_point_t ship1_mid_index;
static tinygl_point_t ship1_right_index;
// Ship2 position
static tinygl_point_t ship2_left_index;
static tinygl_point_t ship2_mid_index;
static tinygl_point_t ship2_right_index;
// Ship3 position
static tinygl_point_t ship3_left_index;
static tinygl_point_t ship3_mid_index;
static tinygl_point_t ship3_right_index;
// Number of ships
static uint8_t no_ships = 0;

// static tinygl_point_t ships[] =
// {
//   ship1_left_index,
//   ship1_mid_index,
//   ship1_right_index,
//
//   ship2_left_index,
//   ship2_mid_index,
//   ship2_right_index,
//
//   ship3_left_index,
//   ship3_mid_index,
//   ship3_right_index
// }

// static uint8_t my_ship_count = 0;
// static uint8_t ur_ship_count = 0;
// static uint8_t p = 1;
// static uint8_t map_view = 0;

// static uint8_t ship_map[] =
// {
//     0x0, 0x0, 0x0, 0x0, 0x0
// };
//
// static uint8_t hit_map[] =
// {
//     0x0, 0x0, 0x0, 0x0, 0x0
// };

static uint8_t check_index(tinygl_point_t ship_index)
{
    uint8_t i;
    if (ship1_left_index.x == ship_index.x && ship1_left_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship1_mid_index.x == ship_index.x && ship1_mid_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship1_right_index.x == ship_index.x && ship1_right_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship2_left_index.x == ship_index.x && ship2_left_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship2_mid_index.x == ship_index.x && ship2_mid_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship2_right_index.x == ship_index.x && ship2_right_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship3_left_index.x == ship_index.x && ship3_left_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship3_mid_index.x == ship_index.x && ship3_mid_index.y == ship_index.y)
    {
        return 1;
    }

    if (ship3_right_index.x == ship_index.x && ship3_right_index.y == ship_index.y)
    {
        return 1;
    }
    return 0;
}

static void planning_phase (void)
{
    uint8_t s_x = 2;
    uint8_t s_mid_y = 3;

    uint8_t set_ship = 0;
    // uint8_t ship_placed = 0;

    // uint8_t i;

    // for (i = 0; i < 3; i++)
    // {
    // Initialise
    tinygl_point_t ship_left_index = tinygl_point (s_x, s_mid_y+1);
    tinygl_point_t ship_mid_index = tinygl_point (s_x, s_mid_y);
    tinygl_point_t ship_right_index = tinygl_point (s_x, s_mid_y-1);
    tinygl_pixel_set (ship_left_index, 1);
    tinygl_pixel_set (ship_mid_index, 1);
    tinygl_pixel_set (ship_right_index, 1);

    // Move ship
    while (1 && no_ships < 3)
    {
        navswitch_update ();
        tinygl_update ();

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && s_mid_y > 1)
        {
            if (!check_index(ship_left_index))
            {
                tinygl_pixel_set (ship_left_index, 0);
            }

            s_mid_y--;
            ship_left_index = tinygl_point (s_x, s_mid_y+1);
            ship_mid_index = tinygl_point (s_x, s_mid_y);
            ship_right_index = tinygl_point (s_x, s_mid_y-1);
            tinygl_pixel_set (ship_left_index, 1);
            tinygl_pixel_set (ship_mid_index, 1);
            tinygl_pixel_set (ship_right_index, 1);
            set_ship = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && s_mid_y < 5)
        {
            if (!check_index(ship_right_index))
            {
              tinygl_pixel_set (ship_right_index, 0);
            }
            s_mid_y++;
            ship_left_index = tinygl_point (s_x, s_mid_y+1);
            ship_mid_index = tinygl_point (s_x, s_mid_y);
            ship_right_index = tinygl_point (s_x, s_mid_y-1);
            tinygl_pixel_set (ship_left_index, 1);
            tinygl_pixel_set (ship_mid_index, 1);
            tinygl_pixel_set (ship_right_index, 1);
            set_ship = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST) && s_x < 4)
        {
            if (!set_ship)
            {
                tinygl_pixel_set (ship_left_index, 0);
                tinygl_pixel_set (ship_mid_index, 0);
                tinygl_pixel_set (ship_right_index, 0);
            }
            s_x++;
            ship_left_index = tinygl_point (s_x, s_mid_y+1);
            ship_mid_index = tinygl_point (s_x, s_mid_y);
            ship_right_index = tinygl_point (s_x, s_mid_y-1);
            tinygl_pixel_set (ship_left_index, 1);
            tinygl_pixel_set (ship_mid_index, 1);
            tinygl_pixel_set (ship_right_index, 1);
            set_ship = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST) && s_x > 0)
        {
            if (!set_ship)
            {
                tinygl_pixel_set (ship_left_index, 0);
                tinygl_pixel_set (ship_mid_index, 0);
                tinygl_pixel_set (ship_right_index, 0);
            }
            s_x--;
            ship_left_index = tinygl_point (s_x, s_mid_y+1);
            ship_mid_index = tinygl_point (s_x, s_mid_y);
            ship_right_index = tinygl_point (s_x, s_mid_y-1);
            tinygl_pixel_set (ship_left_index, 1);
            tinygl_pixel_set (ship_mid_index, 1);
            tinygl_pixel_set (ship_right_index, 1);
            set_ship = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH))
        {
            no_ships++;
            set_ship = 1;
            if (no_ships == 1)
            {
                ship1_left_index = ship_left_index;
                ship1_mid_index = ship_mid_index;
                ship1_right_index = ship_right_index;
                tinygl_pixel_set (ship1_left_index, 1);
                tinygl_pixel_set (ship1_mid_index, 1);
                tinygl_pixel_set (ship1_right_index, 1);
            }
            else if (no_ships == 2)
            {
                ship2_left_index = ship_left_index;
                ship2_mid_index = ship_mid_index;
                ship2_right_index = ship_right_index;
                tinygl_pixel_set (ship2_left_index, 1);
                tinygl_pixel_set (ship2_mid_index, 1);
                tinygl_pixel_set (ship2_right_index, 1);
            }
            else if (no_ships == 3)
            {
                ship3_left_index = ship_left_index;
                ship3_mid_index = ship_mid_index;
                ship3_right_index = ship_right_index;
                tinygl_pixel_set (ship3_left_index, 1);
                tinygl_pixel_set (ship3_mid_index, 1);
                tinygl_pixel_set (ship3_right_index, 1);
            }
        }
    }
    // }
}

int main (void)
{
    system_init ();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    navswitch_init ();
    // ir_uart_init ();

    planning_phase();

    while (1)
    {
        // tinygl_text ("DONE");
        tinygl_update ();
    }
    // while (1)
    // {
    //     if (p == 1)
    //     {
    //         tinygl_update ();
    //         navswitch_update ();
    //     }
    //     else
    //     {
    //
    //     }
    // }
}
