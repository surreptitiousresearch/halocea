#pragma once
/* game_engine_playlist_s @ game_engine_playlist — the active multiplayer playlist and its cursor. */

#include "game_engine_playlist_item_s.h"

typedef struct game_engine_playlist_s
{
    game_engine_playlist_item_s *playlist;        /* 0x00 */
    unsigned int                 playlist_count;  /* 0x04 */
    unsigned int                 playlist_max_count; /* 0x08 — allocated capacity */
    unsigned int                 now_playing;     /* 0x0C */
} game_engine_playlist_s;

extern game_engine_playlist_s game_engine_playlist;
