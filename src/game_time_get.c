/* game_time_get @ 0x8369A868 — current local game tick */
#include "headers/game_time_globals.h"
int game_time_get(void)
{
    return game_time_globals->local_time;
}
