#pragma once
/* breakable_surface_particle_effect — one element of breakable_surface.particle_effects
   (128 bytes; DB-verified). */

#include "tag_reference.h"
#include "real_argb_color.h"

typedef struct breakable_surface_particle_effect
{
    tag_reference   particle;                      /* 0x00 */
    unsigned int    flags;                         /* 0x10 */
    float           density;                       /* 0x14 */
    float           velocity_scale_lower_bound;    /* 0x18 */
    float           velocity_scale_upper_bound;    /* 0x1C */
    float           velocity_cone_angle;           /* 0x20 */
    float           angular_velocity_lower_bound;  /* 0x24 */
    float           angular_velocity_upper_bound;  /* 0x28 */
    unsigned int    unused1[2];                    /* 0x2C */
    float           radius_lower_bound;            /* 0x34 */
    float           radius_upper_bound;            /* 0x38 */
    unsigned int    unused2[2];                    /* 0x3C */
    real_argb_color tint_lower_bound;              /* 0x44 */
    real_argb_color tint_upper_bound;              /* 0x54 */
    unsigned int    unused3[7];                    /* 0x64 */
} breakable_surface_particle_effect;               /* 128 bytes */
