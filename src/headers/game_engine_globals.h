#pragma once
/* _game_engine_globals @ game_engine_globals — multiplayer engine runtime state
 * (game_engine.c). Full 36-byte layout. */

#include "game_engine_mode.h"

// DB-verified via types_members _game_engine_globals (Blam spelling: game_engine_globals_t)
typedef struct _game_engine_globals
{
    int              flags;                                        /* 0x00 */
    int              current_team_to_add;                          /* 0x04 */
    float            game_ending_timer;                            /* 0x08 */
    float            post_game_options_fade;                       /* 0x0C */
    game_engine_mode mode;                                         /* 0x10 */
    float            score_alpha[2];                               /* 0x14 */
    unsigned char    map_cycle_countdown_enabled;                  /* 0x1C */
    unsigned char    _pad1D[3];                                   /* 0x1D */
    float            time_until_map_cycle_timeout_expires_seconds; /* 0x20 */
} game_engine_globals_t;                                           /* 36 bytes */

extern game_engine_globals_t game_engine_globals;
