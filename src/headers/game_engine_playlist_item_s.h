#pragma once
/* game_engine_playlist_item_s — one entry of the multiplayer map/variant playlist (164 bytes). */

#include "game_variant.h"

typedef struct game_engine_playlist_item_s
{
    char        *map_name;       /* 0x00 */
    char        *variant_name;   /* 0x04 */
    int          max_players;    /* 0x08 */
    game_variant variant;        /* 0x0C (152 bytes) */
} game_engine_playlist_item_s;   /* 0xA4 */
