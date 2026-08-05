#pragma once
/* _player_ui_globals @ player_ui_globals — per-local-player UI/profile state
 * (player_ui.c). DB _player_ui_globals, 8108 bytes. Profile/edit sub-blocks and the
 * per-player slot are defined in their own DB-verified headers, included below. */

#include <stdint.h>
#include "_player_data.h"        /* _player_data (pulls in player_profile) */
#include "_edit_profile_data.h"  /* _edit_profile_data */
#include "game_globals.h"        /* game_variant */

typedef struct _player_ui_globals
{
    _player_data       player_data[2];                          /* 0x0000 */
    unsigned char      join_players_to_next_multiplayer_game[2];/* 0x0F88 */
    int16_t            single_player_controller_index[2];       /* 0x0F8A */
    unsigned char      _pad0F8E[2];                             /* 0x0F8E */
    game_variant       variant;                                 /* 0x0F90 */
    unsigned char      user_selected_variant;                   /* 0x1028 */
    unsigned char      _pad1029[3];                             /* 0x1029 */
    _edit_profile_data edit_profile_data;                       /* 0x102C */
    unsigned char      initialized;                             /* 0x1FA8 */
    unsigned char      _pad1FA9[3];                             /* 0x1FA9 — trailing alignment */
} _player_ui_globals;                                           /* 8108 bytes */

extern _player_ui_globals player_ui_globals;
