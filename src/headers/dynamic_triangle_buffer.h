#pragma once
/* dynamic_triangle_buffer — one sub-allocation within the per-frame dynamic index-buffer pool (12 bytes). */

struct rasterizer_triangle;

typedef struct dynamic_triangle_buffer
{
    int                          triangle_start_ofs; /* 0x0 — 3 * starting triangle index */
    int                          triangle_count;     /* 0x4 */
    struct rasterizer_triangle  *data;               /* 0x8 */
} dynamic_triangle_buffer;                            /* 12 bytes */
