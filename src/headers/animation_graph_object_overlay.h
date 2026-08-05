#pragma once
#include <stdint.h>
/* animation_graph_object_overlay — one element of an animation graph's object_overlays tag block
 * (20 bytes): an overlay animation driven by an object function value. Layout from the database
 * (types_members). */

typedef struct animation_graph_object_overlay
{
    int16_t          animation_index;  /* 0x00 — -1 = none */
    int16_t          function_index;   /* 0x02 — object function driving the overlay */
    int16_t          mode;             /* 0x04 — 0 = continuous (function maps to frame), 1 = scaled */
    uint16_t pad;              /* 0x06 */
    int              unused[3];        /* 0x08 */
} animation_graph_object_overlay;      /* 20 bytes */
