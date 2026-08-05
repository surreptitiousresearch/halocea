#pragma once
/* firing_position — one candidate AI firing position with its pathfinding/scoring state. DB layout. */

#include <stdint.h>
#include "real_vector3d.h"
#include "firing_position_definition.h"

typedef struct firing_position
{
    firing_position_definition *definition;                 /* 0x00 */
    int16_t        original_index;                           /* 0x04 */
    int16_t        line_of_sight;                            /* 0x06 */
    float          path_distance_from_actor;                 /* 0x08 */
    real_vector3d  path_direction_from_actor;                /* 0x0C */
    float          path_distance_to_target;                  /* 0x18 */
    float          path_closest_approach_to_target;          /* 0x1C */
    real_vector3d  path_direction_from_target;               /* 0x20 */
    float          linear_distance_squared_to_target;        /* 0x2C */
    unsigned char  valid;                                    /* 0x30 */
    unsigned char  rejected;                                 /* 0x31 */
    unsigned char _pad0[2]; /* db-verified padding */
    float          pre_evaluation;                           /* 0x34 */
    float          evaluation;                               /* 0x38 */
} firing_position;                                           /* 0x3C (60 bytes) */
