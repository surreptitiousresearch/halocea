#pragma once

/* animation_graph_vehicle_suspension_animation — element of
 * animation_graph_vehicle_animations.suspension_animations (size 0x14) */
typedef struct animation_graph_vehicle_suspension_animation
{
    __int16 mass_point_index;                 /* 0x00 */
    __int16 animation_index;                  /* 0x02 */
    float   full_extension_ground_depth;      /* 0x04 */
    float   full_compression_ground_depth;    /* 0x08 */
    int     unused[2];                        /* 0x0C */
} animation_graph_vehicle_suspension_animation;
