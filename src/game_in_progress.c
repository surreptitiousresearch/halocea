/* game_in_progress @ 0x8369A8C0 — a map is loaded and the clock is running or paused */
#include <stdint.h>
#include "headers/game_time_globals.h"

/* Blam boolean: returns 0/1 (li r3 / subfe); every caller does clrlwi rN,r3,24 -> uint8_t. */
uint8_t game_in_progress(void)
{
    if ( !game_time_globals || !game_time_globals->initialized )
        return 0;
    if ( game_time_globals->active )
        return 1;
    return game_time_globals->paused != 0;
}
