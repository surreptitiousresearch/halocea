#pragma once
/* struct_game_allegiance_globals — runtime tracking of AI team alliances and the
 * incidents (e.g. friendly fire) that can break them (game_allegiance.c). */

#include "game_allegiance.h"

typedef struct struct_game_allegiance_globals
{
    __int16         allegiance_count;      /* 0x00 */
    game_allegiance allegiances[8];        /* 0x02 */
    char            _pad_0x92[2];          /* 0x92 (alignment pad before ally_bitvector) */
    unsigned int    ally_bitvector[4];     /* 0x94 */
    unsigned int    friendly_bitvector[4]; /* 0xA4 */
} struct_game_allegiance_globals;          /* 180 bytes */

extern struct_game_allegiance_globals *game_allegiance_globals;
