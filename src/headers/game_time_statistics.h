#pragma once
/* game_time_statistics @ 0x84420968 — per-frame timing/latency accumulator dumped to `file`.
 * Full layout per DB types_members(game_time_statistics); 40 bytes. */

#include <stdint.h>
#include <stdio.h>   /* FILE (_iobuf) */

struct game_time_statistics
{
    FILE            *file;                             /* 0x00 */
    unsigned char    active;                           /* 0x04 */
    unsigned char    first_line;                        /* 0x05 */
    unsigned char   _pad06[2];                          /* 0x06 */
    unsigned int     last_milliseconds;                 /* 0x08 */
    int16_t          frame_count;                        /* 0x0C */
    int16_t          total_milliseconds_elapsed;         /* 0x0E */
    int16_t          minimum_milliseconds_per_frame;     /* 0x10 */
    int16_t          maximum_milliseconds_per_frame;     /* 0x12 */
    int16_t          total_latency;                       /* 0x14 */
    int16_t          minimum_latency;                     /* 0x16 */
    int16_t          maximum_latency;                      /* 0x18 */
    int16_t          server_updates;                       /* 0x1A */
    int16_t          minimum_server_updates;               /* 0x1C */
    int16_t          maximum_server_updates;               /* 0x1E */
    int16_t          predicted_updates;                    /* 0x20 */
    int16_t          minimum_predicted_updates;            /* 0x22 */
    int16_t          maximum_predicted_updates;            /* 0x24 */
    unsigned char   _pad26[2];                             /* 0x26 — pad to 40 */
}; /* 40 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern struct game_time_statistics game_time_statistics;

#ifdef __cplusplus
}
#endif
