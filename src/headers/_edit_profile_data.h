#pragma once
/* _edit_profile_data — in-progress profile/variant edit buffers (player_ui.c). DB
 * _edit_profile_data, 3964 bytes.
 *
 * edit_profile (0x004) and original_profile (0x7C0) share one anonymous-union DB type
 * (_5561023CC6002D86B081B46AEF2B8FEB, 1980 bytes): viewed as a player_profile when editing a
 * controller/input profile, or as a game_variant when editing a multiplayer game variant.
 * $-name kept verbatim ($ in identifiers is an MSVC extension), matching the mtlPARAM.h
 * convention. */

#include "player_profile.h"
#include "game_variant.h"

/* DB types_members _5561023CC6002D86B081B46AEF2B8FEB, 1980 bytes (max member) */
typedef union _5561023CC6002D86B081B46AEF2B8FEB
{
    player_profile player_profile;       /* 0x000, 1980 bytes */
    game_variant   variant;              /* 0x000, 152 bytes  */
} _5561023CC6002D86B081B46AEF2B8FEB;

typedef struct _edit_profile_data
{
    int                               profile_index;     /* 0x000 */
    _5561023CC6002D86B081B46AEF2B8FEB edit_profile;      /* 0x004 */
    _5561023CC6002D86B081B46AEF2B8FEB original_profile;  /* 0x7C0 */
} _edit_profile_data;                    /* 3964 bytes */
