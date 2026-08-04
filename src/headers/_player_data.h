#pragma once
/* _player_data — one local player's UI slot: the saved player_profile plus join/
 * bookkeeping fields (player_ui.c). DB _player_data, 1988 bytes. */

#include "player_profile.h"  /* player_profile */

typedef struct _player_data
{
    player_profile profile;                 /* 0x000 */
    int            profile_index;           /* 0x7BC */
    unsigned char  prejoined_multiplayer;   /* 0x7C0 */
    unsigned char  _pad7C1[3];              /* 0x7C1 pad to 1988 */
} _player_data;                             /* 1988 bytes */
