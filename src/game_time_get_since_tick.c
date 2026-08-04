/* game_time_get_since_tick @0x8369A888 — fractional time elapsed since the last simulation tick, in seconds.
 *
 * Deviation: the decompiler rendered the single `lfs f1, 0x1C(r11)` load as a double read of the high float;
 * the field is a float (leftover_time_sec @0x1C) returned directly. */

#include "headers/game_time_globals.h"

float game_time_get_since_tick(void)
{
    return game_time_globals->leftover_time_sec;
}
