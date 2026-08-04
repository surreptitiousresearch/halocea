#pragma once
/* player_score_network_data — the replicated per-player score payload of a player_score_update message
 * (40 bytes, DB layout). */

#include "multiplayer_statistics.h"

typedef struct player_score_network_data
{
    __int16                kills[4];            /* 0x00 */
    __int16                assists[4];          /* 0x08 */
    __int16                friendly_fire_kills; /* 0x10 */
    __int16                deaths;              /* 0x12 */
    __int16                suicides;            /* 0x14 */
    multiplayer_statistics multiplayer_stats;   /* 0x16 */
    int                    special;             /* 0x1C */
    unsigned __int8        is_odd_man_out;      /* 0x20 */
    unsigned char          _pad21[3];           /* 0x21 */
    float                  speed_multiplier;    /* 0x24 */
} player_score_network_data;                    /* 40 bytes */
