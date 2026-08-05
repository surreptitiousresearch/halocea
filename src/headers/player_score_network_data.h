#pragma once
/* player_score_network_data — the replicated per-player score payload of a player_score_update message
 * (40 bytes, DB layout). */

#include <stdint.h>
#include "multiplayer_statistics.h"

typedef struct player_score_network_data
{
    int16_t                kills[4];            /* 0x00 */
    int16_t                assists[4];          /* 0x08 */
    int16_t                friendly_fire_kills; /* 0x10 */
    int16_t                deaths;              /* 0x12 */
    int16_t                suicides;            /* 0x14 */
    multiplayer_statistics multiplayer_stats;   /* 0x16 */
    int                    special;             /* 0x1C */
    uint8_t        is_odd_man_out;      /* 0x20 */
    unsigned char          _pad21[3];           /* 0x21 */
    float                  speed_multiplier;    /* 0x24 */
} player_score_network_data;                    /* 40 bytes */
