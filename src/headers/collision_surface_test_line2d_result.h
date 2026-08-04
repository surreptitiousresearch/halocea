#pragma once
/* collision_surface_test_line2d_result — output of collision_surface_test_line2d: where a 2D line enters and
 * exits the given surface's boundary loop (24 bytes, types_members-confirmed layout, DB ordinal 41506). */

typedef struct collision_surface_test_line2d_result
{
    float enter_t;             /* 0x00 */
    int   enter_edge_index;    /* 0x04 */
    int   enter_surface_index; /* 0x08 */
    float exit_t;               /* 0x0C */
    int   exit_edge_index;      /* 0x10 */
    int   exit_surface_index;   /* 0x14 */
} collision_surface_test_line2d_result; /* 24 bytes */
