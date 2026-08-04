#pragma once

/* actor_position_data — sampled per-frame position/orientation snapshot for an actor (56 bytes).
 * Full DB layout (types_members) — authoritative over write-order in actor_input_sample_position. */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "location.h"

typedef struct actor_position_data
{
    real_point3d  head_position;   /* 0x00 */
    real_point3d  body_position;   /* 0x0C */
    real_vector3d body_facing;     /* 0x18 */
    location      body_location;   /* 0x24 (8 bytes) */
    real_vector3d velocity;        /* 0x2C */
} actor_position_data;             /* 0x38 */
