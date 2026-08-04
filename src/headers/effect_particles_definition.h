#pragma once
/* effect_particles_definition — one element of effect_event_definition.particles (232 bytes;
   DB-verified, matches headers_ref). */

#include "tag_reference.h"
#include "real_euler_angles2d.h"
#include "real_vector3d.h"
#include "real_argb_color.h"

typedef struct effect_particles_definition
{
    __int16            environment;                     /* 0x00 */
    __int16            disposition;                     /* 0x02 */
    __int16            camera_mode;                     /* 0x04 */
    unsigned __int16   pad;                             /* 0x06 */
    __int16            location_index;                  /* 0x08 */
    unsigned __int16   pad2;                            /* 0x0A */
    real_euler_angles2d direction;                     /* 0x0C */
    real_vector3d      offset;                          /* 0x14 */
    real_vector3d      runtime_direction;               /* 0x20 */
    int                unused1[10];                     /* 0x2C */
    tag_reference      particle;                        /* 0x54 */
    unsigned int       flags;                           /* 0x64 */
    __int16            distribution_function;           /* 0x68 */
    __int16            pad3;                            /* 0x6A */
    __int16            count_lower_bound;               /* 0x6C */
    __int16            count_upper_bound;               /* 0x6E */
    float              distribution_radius_lower_bound; /* 0x70 */
    float              distribution_radius_upper_bound; /* 0x74 */
    int                unused2[3];                      /* 0x78 */
    float              velocity_lower_bound;            /* 0x84 */
    float              velocity_upper_bound;            /* 0x88 */
    float              velocity_cone_angle;             /* 0x8C */
    float              angular_velocity_lower_bound;    /* 0x90 */
    float              angular_velocity_upper_bound;    /* 0x94 */
    int                unused3[2];                      /* 0x98 */
    float              radius_lower_bound;              /* 0xA0 */
    float              radius_upper_bound;              /* 0xA4 */
    int                unused4[2];                      /* 0xA8 */
    real_argb_color    tint_lower_bound;                /* 0xB0 */
    real_argb_color    tint_upper_bound;                /* 0xC0 */
    int                unused5[4];                      /* 0xD0 */
    unsigned int       a_scales;                        /* 0xE0 */
    unsigned int       b_scales;                        /* 0xE4 */
} effect_particles_definition;                          /* 232 bytes */
