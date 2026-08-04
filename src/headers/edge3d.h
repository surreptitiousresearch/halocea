#pragma once

/* edge3d — one edge of a convex hull under construction (32 bytes, DB types_members-confirmed layout,
 * resolved by convex_hull3d_begin.c, the first real consumer). Each edge stores its two endpoint vertices, the
 * two adjacent edges (one per end), and the two surfaces it borders. */
typedef struct edge3d
{
    unsigned __int8 extant;              /* 0x00 */
    unsigned char _pad0[3]; /* db-verified padding */
    int             vertex_indices[2];   /* 0x04 */
    int             edge_indices[2];     /* 0x0C */
    int             surface_indices[2];  /* 0x14 */
    int             user_data;           /* 0x1C */
} edge3d; /* 32 bytes */
