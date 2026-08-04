#pragma once

#include "real_point3d.h"

/* path_node — one A*-style search node in a path_state's working set (52 bytes, types_members-confirmed
 * field layout; 2 trailing padding bytes after heap_location). */
typedef struct path_node
{
    __int16       child_node_index;               /* 0x00 */
    __int16       parent_node_index;               /* 0x02 */
    int           parent_node_surface_index;        /* 0x04 */
    int           surface_index;                    /* 0x08 */
    real_point3d  entry_point;                      /* 0x0C */
    float         linear_distance_to_entry_point;    /* 0x18 */
    float         closest_approach_to_attractor;     /* 0x1C */
    float         path_distance_from_origin;         /* 0x20 */
    float         cumulative_cost;                   /* 0x24 */
    float         total_cost_estimate;               /* 0x28 */
    __int16       quantized_cost_estimate;           /* 0x2C */
    __int16       depth;                             /* 0x2E */
    __int16       heap_location;                     /* 0x30 */
    unsigned char _pad32[2];                         /* 0x32 */
} path_node; /* 0x34 */
