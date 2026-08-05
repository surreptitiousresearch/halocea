#pragma once
/* object_placement_data — the request structure passed to object_new() describing where and how to place
 * a new object. 136 bytes; layout from the database. */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_rgb_color.h"

typedef struct object_placement_data
{
    int             definition_index;                /* 0x00 */
    unsigned int    flags;                           /* 0x04 */
    int             owner_player_index;              /* 0x08 */
    int             owner_object_index;              /* 0x0C */
    int             owner_object_definition_index;   /* 0x10 */
    int16_t         owner_team_index;                /* 0x14 */
    int16_t         variant_number;                  /* 0x16 */
    real_point3d    position;                        /* 0x18 */
    float           height;                          /* 0x24 */
    real_vector3d   translational_velocity;          /* 0x28 */
    real_vector3d   forward;                         /* 0x34 */
    real_vector3d   up;                              /* 0x40 */
    real_vector3d   angular_velocity;                /* 0x4C */
    real_rgb_color  change_colors[4];                /* 0x58 — 4 × 12 bytes */
} object_placement_data;                             /* 136 bytes */
