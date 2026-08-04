#pragma once
/* animation_graph_object_overlay — one element of an animation graph's object_overlays tag block
 * (20 bytes): an overlay animation driven by an object function value. Layout from the database
 * (types_members). */

typedef struct animation_graph_object_overlay
{
    __int16          animation_index;  /* 0x00 — -1 = none */
    __int16          function_index;   /* 0x02 — object function driving the overlay */
    __int16          mode;             /* 0x04 — 0 = continuous (function maps to frame), 1 = scaled */
    unsigned __int16 pad;              /* 0x06 */
    int              unused[3];        /* 0x08 */
} animation_graph_object_overlay;      /* 20 bytes */
