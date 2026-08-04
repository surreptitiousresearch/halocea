#pragma once
/* collision_edge — winged-edge record in the collision BSP (24 bytes). DB layout verbatim: three
 * int[2] pairs. Index [0]/[1] meanings: vertex_indices = start/end vertex, edge_indices =
 * forward/reverse edge, surface_indices = left/right surface. */

typedef struct collision_edge
{
    int vertex_indices[2];  /* 0x00 — [0] start, [1] end */
    int edge_indices[2];    /* 0x08 — [0] forward, [1] reverse */
    int surface_indices[2]; /* 0x10 — [0] left, [1] right */
} collision_edge;           /* 24 bytes */
