#pragma once
/* particle_system_datum — one live particle system (particle_systems.c). 344 bytes; layout from the
 * database. */

#include <stdint.h>
#include "location.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_argb_color.h"
#include "real_rgb_color.h"
#include "system_physics_state.h"
#include "particle_type.h"

typedef struct particle_system_datum
{
    int16_t              identifier;        /* 0x00 */
    unsigned char _pad0[2]; /* db-verified padding */
    unsigned int         flags;             /* 0x04 — particle_system_flags: bit0 _particle_system_active_bit,
                                                bit1 _particle_system_initializing_bit (DB $B480F3D5302DDD26DD58C5FA84DA868D) */
    int                  definition_index;  /* 0x08 */
    int                  object_index;      /* 0x0C — host object, -1 if free */
    int16_t              attachment_index;  /* 0x10 */
    int16_t              function_index;    /* 0x12 — drives scale */
    float                scale;             /* 0x14 */
    location             location;          /* 0x18 */
    real_point3d         position;          /* 0x20 */
    real_vector3d        velocity;          /* 0x2C */
    real_argb_color      color;             /* 0x38 */
    real_rgb_color       lighting;          /* 0x48 */
    system_physics_state physics_states;    /* 0x54 */
    particle_type        types[4];          /* 0x58 */
} particle_system_datum;                    /* 344 bytes */
