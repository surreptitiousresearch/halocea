#pragma once
/* path_test_pill2d_result — output of path_test_pill2d (16 bytes, types_members-confirmed layout). */

typedef struct path_test_pill2d_result
{
    float    distance;        /* 0x00 */
    int      surface_index;   /* 0x04 */
    int      edge_index;      /* 0x08 */
    __int16  disc_index;      /* 0x0C */
    __int16  obstacle_index;  /* 0x0E */
} path_test_pill2d_result; /* 16 bytes */
