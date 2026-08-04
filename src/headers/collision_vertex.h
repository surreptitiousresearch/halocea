#pragma once

#include "real_point3d.h"

typedef struct collision_vertex
{
    real_point3d point;            /* 0x00 */
    int          first_edge_index; /* 0x0C */
} collision_vertex;                /* 16 bytes */
