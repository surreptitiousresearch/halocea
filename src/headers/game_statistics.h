#pragma once
/* game_statistics — per-player lifetime/session statistics record (58 bytes + 2 pad, DB layout). */

#include <stdint.h>
#include "multiplayer_statistics.h"

typedef struct game_statistics
{
    int16_t sort_key;                         /* 0x00 */
    int16_t games_played;                     /* 0x02 */
    int16_t games_won;                        /* 0x04 */
    int16_t kills_in_a_row;                   /* 0x06 */
    int16_t multiple_kills;                   /* 0x08 */
    int16_t last_kill_time;                   /* 0x0A */
    int16_t kills[4];                         /* 0x0C */
    int16_t assists[4];                       /* 0x14 */
    int16_t friendly_fire_kills;              /* 0x1C */
    int16_t deaths;                           /* 0x1E */
    int16_t suicides;                         /* 0x20 */
    char    _pad22[2];                         /* 0x22 */
    int     shots_fired;                      /* 0x24 */
    int     shots_hit;                        /* 0x28 */
    int     seconds_online;                   /* 0x2C */
    int16_t killed_teammate_since_last_death; /* 0x30 */
    int16_t custom_data_size;                 /* 0x32 */
    multiplayer_statistics multiplayer_statistics; /* 0x34 */
    char    _pad3A[2];                        /* 0x3A */
} game_statistics; /* 60 bytes */
