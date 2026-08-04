#pragma once
/* animation_graph_unit_seat — one element of an animation_graph's unit_seats tag_block (100 bytes): a unit
 * seat's animation set, ik points, per-weapon-class animation overrides, and its own fixed looking-screen
 * bounds. Layout DB-verified. */

#include "tag_block.h"
#include "animation_aiming_screen_bounds.h"

typedef struct animation_graph_unit_seat
{
    char                            label[32];              /* 0x00 */
    animation_aiming_screen_bounds  looking_screen_bounds;   /* 0x20 (32) */
    int                             unused[2];               /* 0x38 (56) */
    tag_block                       animations;              /* 0x40 (64) */
    tag_block                       ik_points;               /* 0x4C (76) */
    tag_block                       weapon_classes;          /* 0x58 (88) */
} animation_graph_unit_seat;                                  /* 0x64 = 100 bytes */
