#pragma once

/* actor_burst_geometry — computed burst aiming geometry (44 bytes, DB-verified layout). */
typedef struct actor_burst_geometry
{
    float burst_origin_radius;                /* 0x00 */
    float burst_origin_angle;                 /* 0x04 */
    float burst_return_length_lower_bound;    /* 0x08 */
    float burst_return_length_upper_bound;    /* 0x0C */
    float burst_return_angle;                 /* 0x10 */
    float burst_duration_lower_bound;         /* 0x14 */
    float burst_duration_upper_bound;         /* 0x18 */
    float burst_separation_lower_bound;       /* 0x1C */
    float burst_separation_upper_bound;       /* 0x20 */
    float burst_maximum_angular_velocity;     /* 0x24 */
    unsigned int unused;                      /* 0x28 */
} actor_burst_geometry;                       /* 44 bytes */
