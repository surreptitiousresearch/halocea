#pragma once
/* game_time_statistics @ 0x84420968 — per-frame timing/latency accumulator dumped to `file`.
 * Full layout per DB types_members(game_time_statistics); 40 bytes. */

#include <stdio.h>   /* FILE (_iobuf) */

struct game_time_statistics
{
    FILE            *file;                             /* 0x00 */
    unsigned char    active;                           /* 0x04 */
    unsigned char    first_line;                        /* 0x05 */
    unsigned char   _pad06[2];                          /* 0x06 */
    unsigned int     last_milliseconds;                 /* 0x08 */
    __int16          frame_count;                        /* 0x0C */
    __int16          total_milliseconds_elapsed;         /* 0x0E */
    __int16          minimum_milliseconds_per_frame;     /* 0x10 */
    __int16          maximum_milliseconds_per_frame;     /* 0x12 */
    __int16          total_latency;                       /* 0x14 */
    __int16          minimum_latency;                     /* 0x16 */
    __int16          maximum_latency;                      /* 0x18 */
    __int16          server_updates;                       /* 0x1A */
    __int16          minimum_server_updates;               /* 0x1C */
    __int16          maximum_server_updates;               /* 0x1E */
    __int16          predicted_updates;                    /* 0x20 */
    __int16          minimum_predicted_updates;            /* 0x22 */
    __int16          maximum_predicted_updates;            /* 0x24 */
    unsigned char   _pad26[2];                             /* 0x26 — pad to 40 */
}; /* 40 bytes */

extern struct game_time_statistics game_time_statistics;
