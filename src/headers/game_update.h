#pragma once
/* game_update — the per-tick block of player actions sent in one network update, plus the per-player
 * id of the most recent client update each action acknowledges. */

#include "player_action.h"

typedef struct game_update
{
    unsigned __int16 number_of_actions;            /* 0x000 */
    char             _pad2[2];                      /* 0x002 */
    player_action    actions[32];                   /* 0x004 */
    int              completed_client_update_id[32];/* 0x404 */
} game_update;                                       /* 1156 bytes */
