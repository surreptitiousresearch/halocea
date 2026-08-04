/* game_time_get_elapsed @0x8369A878 — number of game ticks that elapsed during the last local frame. */

#include <stdint.h>

#include "headers/game_time_globals.h"

/* lhz 0x10 loads the 16-bit field; every caller re-normalizes with extsh -> signed 16-bit return.
 * Removed the (unsigned __int16) papering cast: the __int16 field is returned directly. */
int16_t game_time_get_elapsed(void)
{
    return game_time_globals->last_local_time_elapsed;
}
