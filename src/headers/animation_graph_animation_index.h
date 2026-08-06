#pragma once
/* animation_graph_animation_index — one element of an animation-graph tag_block whose whole payload is a
 * single animation index (animation_graph_unit_seat.animations, animation_graph_device_animations.animations,
 * the unit-damage/weapon-class animation tables …). The index selects an element of animation_graph.animations;
 * -1 means "no animation for this slot". DB type animation_graph_animation_index (types_members, 2 bytes). */

#include <stdint.h>

typedef struct animation_graph_animation_index
{
    int16_t animation_index;           /* 0x00 */
} animation_graph_animation_index;      /* 2 bytes */
