#pragma once
/* fg_graph — one scrolling framerate graph (12976 bytes / 0x32B0). Holds the plotted line as 512
 * dynamic-screen vertices (only the y component is animated; the x grid is laid out once by fg_init),
 * a 5-vertex border strip, the running 4-sample average window, and the on-screen label text. Layout
 * from the database. */

#include "rectangle2d.h"
#include "dynamic_screen_vertex.h"

typedef struct fg_graph
{
    rectangle2d           extents;            /* 0x0000 — plot box; n[2] is the baseline y */
    rectangle2d           label_extents;      /* 0x0008 */
    rectangle2d           label_max_extents;  /* 0x0010 */
    rectangle2d           label_avg_extents;  /* 0x0018 */
    dynamic_screen_vertex vertices[512];      /* 0x0020 — the plotted line */
    dynamic_screen_vertex border[5];          /* 0x3020 */
    float                 max;                /* 0x3098 — full-scale sample value */
    float                 average;            /* 0x309C */
    float                 average_samples[4]; /* 0x30A0 — sliding window */
    char                  label[512];         /* 0x30B0 */
} fg_graph;                                   /* 0x32B0 */
