/* game_map_loading_in_progress @0x83687A08 — report whether a map load is underway, optionally returning
 * the current loading progress [0,1] through `progress`. */

#include <stdint.h>
#include "headers/game_globals.h"

uint8_t game_map_loading_in_progress(float *progress)
{
    if ( progress )
        *progress = game_globals->loading_progress;
    return game_globals->map_load_in_progress;
}
