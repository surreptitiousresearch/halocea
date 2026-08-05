#pragma once
/* projectile_material_response_definition — one entry of a projectile definition's material_responses tag
 * block: how the projectile reacts (effects, penetration, reflection) when it hits a given material type.
 * Layout from the database (160 bytes). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct projectile_material_response_definition
{
    uint16_t flags;                              /* 0x00 */
    int16_t          default_response;                   /* 0x02 */
    tag_reference    default_effect;                     /* 0x04 */
    int              unused0[4];                         /* 0x14 */
    int16_t          possible_response;                  /* 0x24 */
    uint16_t possible_response_flags;            /* 0x26 */
    float            possible_response_skip_fraction;    /* 0x28 */
    float            possible_response_minimum_angle;    /* 0x2C */
    float            possible_response_maximum_angle;    /* 0x30 */
    float            possible_response_minimum_velocity; /* 0x34 */
    float            possible_response_maximum_velocity; /* 0x38 */
    tag_reference    possible_response_effect;           /* 0x3C */
    int              unused1[4];                         /* 0x4C */
    int16_t          scale_effects_by;                   /* 0x5C */
    uint16_t pad;                                /* 0x5E */
    float            angle_noise;                        /* 0x60 */
    float            velocity_noise;                     /* 0x64 */
    tag_reference    detonation_effect;                  /* 0x68 */
    int              unused2[6];                         /* 0x78 */
    float            penetration_initial_friction;       /* 0x90 */
    float            penetration_maximum_distance;       /* 0x94 */
    float            reflection_parallel_friction;       /* 0x98 */
    float            reflection_perpendicular_friction;  /* 0x9C */
} projectile_material_response_definition;               /* 160 bytes */
