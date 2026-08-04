/* physics_definition — parsed physics tag: rigid-body bulk properties plus the mass-point tables that
 * describe how the object interacts with ground, water and air. Layout from the database (128 bytes). */
#pragma once

#include "real_point3d.h"
#include "tag_block.h"

typedef struct physics_definition
{
    float        radius;               /* 0x00 */
    float        moment;               /* 0x04 */
    float        mass;                 /* 0x08 */
    real_point3d center_of_mass;       /* 0x0C */
    float        density;              /* 0x18 */
    float        gravity_scale;        /* 0x1C */
    float        ground_friction;      /* 0x20 */
    float        ground_depth;         /* 0x24 */
    float        ground_damp_fraction; /* 0x28 */
    float        ground_normal_k1;     /* 0x2C */
    float        ground_normal_k0;     /* 0x30 */
    float        ground_unused;        /* 0x34 */
    float        water_friction;       /* 0x38 */
    float        water_depth;          /* 0x3C */
    float        water_density;        /* 0x40 */
    float        water_unused;         /* 0x44 */
    float        air_friction;         /* 0x48 */
    float        air_unused;           /* 0x4C */
    float        xx_moment;            /* 0x50 */
    float        yy_moment;            /* 0x54 */
    float        zz_moment;            /* 0x58 */
    tag_block    inertial_matrix;      /* 0x5C — each entry a 36-byte real_matrix3x3; [0]=inertia tensor,
                                           [1]=its inverse (disasm-confirmed, physics_update_new) */
    tag_block    powered_mass_points;  /* 0x68 */
    tag_block    mass_points;          /* 0x74 — each 128 bytes: +56 position, +104 radius (physics_test_point/
                                           _test_vector/_get_features_in_sphere) */
} physics_definition;                  /* 128 bytes */
