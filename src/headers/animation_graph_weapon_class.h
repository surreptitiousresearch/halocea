#pragma once
/* animation_graph_weapon_class — one element of an animation_graph_unit_seat's weapon_classes tag_block
 * (188 bytes): a weapon class's grip/hand marker names, its own aiming-screen bounds, animations, and ik
 * points. Layout DB-verified. */

#include "tag_block.h"
#include "animation_aiming_screen_bounds.h"

typedef struct animation_graph_weapon_class
{
    char                            label[32];            /* 0x00 */
    char                            grip_marker_name[32];  /* 0x20 (32) */
    char                            hand_marker_name[32];  /* 0x40 (64) */
    animation_aiming_screen_bounds  aiming_screen_bounds;   /* 0x60 (96) */
    int                             unused[8];              /* 0x78 (120) */
    tag_block                       animations;              /* 0x98 (152) */
    tag_block                       ik_points;               /* 0xA4 (164) */
    tag_block                       weapon_types;            /* 0xB0 (176) */
} animation_graph_weapon_class;                               /* 0xBC = 188 bytes */
