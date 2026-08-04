#pragma once
/* point_light_geometry_parameters — the geometry block of a point_light_definition (0x30 bytes). The
 * runtime_* fields are precomputed at tag-load time from the authored falloff/cutoff angles. */

typedef struct point_light_geometry_parameters
{
    float radius;                        /* 0x00 */
    float radius_modifier_lower_bound;   /* 0x04 */
    float radius_modifier_upper_bound;   /* 0x08 */
    float falloff_angle;                 /* 0x0C */
    float cutoff_angle;                  /* 0x10 */
    float lens_flare_radius;             /* 0x14 */
    float runtime_cosine_falloff_angle;  /* 0x18 */
    float runtime_cosine_cutoff_angle;   /* 0x1C */
    float specular_radius_multiplier;    /* 0x20 */
    float runtime_sine_cutoff_angle;     /* 0x24 */
    int   unused[2];                     /* 0x28 */
} point_light_geometry_parameters;       /* 0x30 */
