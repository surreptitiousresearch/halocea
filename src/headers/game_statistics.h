#pragma once
/* game_statistics — per-player lifetime/session statistics record (58 bytes + 2 pad, DB layout). */

#include "multiplayer_statistics.h"

typedef struct game_statistics
{
    __int16 sort_key;                         /* 0x00 */
    __int16 games_played;                     /* 0x02 */
    __int16 games_won;                        /* 0x04 */
    __int16 kills_in_a_row;                   /* 0x06 */
    __int16 multiple_kills;                   /* 0x08 */
    __int16 last_kill_time;                   /* 0x0A */
    __int16 kills[4];                         /* 0x0C */
    __int16 assists[4];                       /* 0x14 */
    __int16 friendly_fire_kills;              /* 0x1C */
    __int16 deaths;                           /* 0x1E */
    __int16 suicides;                         /* 0x20 */
    char    _pad22[2];                         /* 0x22 */
    int     shots_fired;                      /* 0x24 */
    int     shots_hit;                        /* 0x28 */
    int     seconds_online;                   /* 0x2C */
    __int16 killed_teammate_since_last_death; /* 0x30 */
    __int16 custom_data_size;                 /* 0x32 */
    multiplayer_statistics multiplayer_statistics; /* 0x34 */
    char    _pad3A[2];                        /* 0x3A */
} game_statistics; /* 60 bytes */
