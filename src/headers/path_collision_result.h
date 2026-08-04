#pragma once

#include "real_point3d.h"

/* path_collision_result — output of structure_test_pill2d (28 bytes, types_members-confirmed layout). */
typedef struct path_collision_result
{
    unsigned __int8 collision;      /* 0x00 */
    unsigned char   _pad1[3];       /* 0x01 */
    real_point3d    point;          /* 0x04 */
    int             surface_index;  /* 0x10 */
    int             edge_index;     /* 0x14 */
    float           t;              /* 0x18 */
} path_collision_result; /* 0x1C */
