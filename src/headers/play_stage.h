#pragma once
/* play_stage @ global_stage — the map/variant selected for the next game to launch. */

#include "game_variant.h"

typedef struct play_stage
{
    char         map_name[64];   /* 0x00 */
    game_variant game_variant;   /* 0x40 (152 bytes) */
} play_stage;                    /* 0xD8 */

extern play_stage global_stage;
