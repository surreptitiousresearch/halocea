#pragma once

#include "player_effect_datum.h"
#include "screen_fade_definition.h"
#include "scripted_player_effect_definition.h"

typedef struct player_effect_globals_definition
{
    player_effect_datum              local_player_effect_data[2]; /* 0x000 (472 bytes) */
    screen_fade_definition           screen_fade;                 /* 0x1D8 (20 bytes) */
    scripted_player_effect_definition scripted_effect;            /* 0x1EC (32 bytes) */
    unsigned int                     global_flags;               /* 0x20C */
    int                              reference_time;              /* 0x210 */
} player_effect_globals_definition;                               /* 532 bytes */

extern player_effect_globals_definition *player_effect_globals;
