#pragma once
/* path_edge — one boundary edge of a pathfinding surface, with its adjacent-surface link. DB layout. */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct path_edge
{
    int              adjacent_surface_index;        /* 0x00 */
    uint8_t  adjacent_pathfinding_surface;   /* 0x04 */
    unsigned char _pad0[3]; /* db-verified padding */
    real_point3d     base_point;                     /* 0x08 */
    real_vector3d    edge_vector;                    /* 0x14 */
} path_edge; /* 32 bytes */
