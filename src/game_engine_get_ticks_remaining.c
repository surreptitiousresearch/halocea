/* game_engine_get_ticks_remaining @0x8374B9F8 — ticks left in the round's time limit, or -1 if the
 * variant has no time limit. Clamped to 0 (never negative) once the limit has been reached. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_variant.h"

extern int game_time_get(void);

unsigned int game_engine_get_ticks_remaining(void)
{
    if (global_variant.universal_variant.time_limit <= 0)
        return (unsigned int)-1;

    int remaining = global_variant.universal_variant.time_limit - game_time_get();
    return remaining >= 0 ? remaining : 0;
}
