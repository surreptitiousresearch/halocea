#pragma once

/* vertex3d — one vertex of a convex hull under construction (12 bytes, DB types_members-confirmed layout,
 * resolved by convex_hull3d_begin.c, the first real consumer). */
typedef struct vertex3d
{
    unsigned __int8 extant;       /* 0x00 */
    unsigned char _pad0[1]; /* db-verified padding */
    __int16         point_index;  /* 0x02 */
    int             edge_index;   /* 0x04 */
    int             user_data;    /* 0x08 */
} vertex3d; /* 12 bytes */
