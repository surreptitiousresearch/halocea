/* game_difficulty_level_get @0x836882B8 — return the current campaign difficulty level (game options). */

#include <stdint.h>
/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_globals.h"


int16_t game_difficulty_level_get(void)
{
    /* (uint16_t) models the lhz (zero-extend) the binary emits on the signed __int16 field. */
    return (uint16_t)game_globals->options.difficulty;
}
