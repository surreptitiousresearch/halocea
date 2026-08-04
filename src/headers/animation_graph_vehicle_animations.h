#pragma once

#include "animation_aiming_screen_bounds.h"
#include "tag_block.h"

/* animation_graph_vehicle_animations — element of animation_graph.vehicle_animations (size 0x74) */
typedef struct animation_graph_vehicle_animations
{
    animation_aiming_screen_bounds steering_screen_bounds;   /* 0x00 */
    int                            unused[17];               /* 0x18 */
    tag_block                      animations;               /* 0x5C */
    tag_block                      suspension_animations;    /* 0x68 */
} animation_graph_vehicle_animations;
