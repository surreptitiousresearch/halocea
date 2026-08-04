/* point_physics_definition — physics tag governing a free-flying point (point_physics.c). Layout from
 * the database. */
#pragma once

typedef struct point_physics_definition
{
    unsigned int flags;                          /* 0x00 */
    float        runtime_mass_over_radius_cubed;  /* 0x04 */
    float        runtime_water_buoyancy_scale;    /* 0x08 */
    float        runtime_air_buoyancy_scale;      /* 0x0C */
    int          unused1[4];                      /* 0x10 */
    float        density;                         /* 0x20 */
    float        air_friction;                    /* 0x24 */
    float        water_friction;                  /* 0x28 */
    float        contact_friction;                /* 0x2C */
    float        elasticity;                      /* 0x30 */
    int          unused2[3];                       /* 0x34 */
} point_physics_definition;                       /* 64 bytes */
