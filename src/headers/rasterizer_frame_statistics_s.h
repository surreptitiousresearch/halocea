#pragma once
/* rasterizer_frame_statistics_s — computed frame-rate statistics for a sampling window. Layout from
 * the database (28 bytes). */

typedef struct rasterizer_frame_statistics_s
{
    float   fps;                /* 0x00 — instantaneous fps over the whole window */
    __int16 fps_sample_count;   /* 0x04 */
    __int16 pad;                /* 0x06 */
    float   fps_average;        /* 0x08 */
    float   fps_min;            /* 0x0C — from the largest frame interval */
    float   fps_max;            /* 0x10 — from the smallest frame interval */
    float   frames_dropped_pct; /* 0x14 */
    int     unused;             /* 0x18 */
} rasterizer_frame_statistics_s;
