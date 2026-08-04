#pragma once
/* player_score_baseline_slot — one entry of the game-engine player score baseline table (48 bytes). */

#include "player_score_network_data.h"

typedef struct player_score_baseline_slot
{
    unsigned char             is_slot_used;   /* 0x00 */
    unsigned char             _pad1[3];       /* 0x01 */
    int                       player_index;   /* 0x04 */
    player_score_network_data baseline;       /* 0x08 — per-player score baseline payload */
} player_score_baseline_slot;                 /* 0x30 */
