#pragma once
#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

/* mass_point_definition — 128 bytes (physics_definition.mass_points block stride). */
typedef struct mass_point_definition
{
    char           name[32];                     /* 0x00 */
    int16_t        powered_mass_point_index;     /* 0x20 */
    int16_t        model_node_index;             /* 0x22 */
    unsigned int   flags;                        /* 0x24 */
    float          relative_mass;                /* 0x28 */
    float          mass;                         /* 0x2C */
    float          relative_density;             /* 0x30 */
    float          density;                      /* 0x34 */
    real_point3d   position;                     /* 0x38 */
    real_vector3d  forward;                      /* 0x44 */
    real_vector3d  up;                           /* 0x50 */
    int16_t        friction_type;                /* 0x5C */
    int16_t        pad;                          /* 0x5E */
    float          friction_parallel_scale;      /* 0x60 */
    float          friction_perpendicular_scale; /* 0x64 */
    float          radius;                       /* 0x68 */
    float          unused[5];                    /* 0x6C */
} mass_point_definition;                          /* 0x80 */
