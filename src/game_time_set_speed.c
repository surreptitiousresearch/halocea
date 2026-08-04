/* game_time_set_speed @0x8369A9A8 — set the global game-time speed multiplier (1.0 = real time). */

#include "headers/game_time_globals.h"
#include "headers/blam_data_globals.h"


void game_time_set_speed(float speed)
{
    game_time_globals->speed = speed;
}
