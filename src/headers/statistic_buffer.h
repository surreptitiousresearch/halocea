#pragma once
/* statistic_buffer — one scoreboard row's numeric fields (28 bytes). DB layout. */

typedef struct statistic_buffer
{
    int player_index; /* 0x00 */
    int score;         /* 0x04 */
    int custom;        /* 0x08 */
    int kills;         /* 0x0C */
    int deaths;        /* 0x10 */
    int assists;       /* 0x14 */
    int place;         /* 0x18 */
} statistic_buffer; /* 28 bytes */
