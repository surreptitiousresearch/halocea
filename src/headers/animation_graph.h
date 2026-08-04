#pragma once
/* animation_graph — the "antr" tag definition (128 bytes, DB layout). Only tag_block members needed by
 * reconstructed source are laid out; the rest is generic tag_block accounting. */

#include "tag_block.h"

typedef struct animation_graph
{
    tag_block object_overlays;                /* 0x00 */
    tag_block unit_seats;                     /* 0x0C */
    tag_block weapon_animations;              /* 0x18 */
    tag_block vehicle_animations;             /* 0x24 */
    tag_block device_animations;              /* 0x30 */
    tag_block unit_damage_animations;         /* 0x3C */
    tag_block first_person_weapon_animations; /* 0x48 */
    tag_block sound_references;               /* 0x54 */
    float     limp_body_node_collision_radius; /* 0x60 */
    unsigned __int16 flags;                    /* 0x64 */
    unsigned __int16 pad;                      /* 0x66 */
    tag_block nodes;                           /* 0x68 */
    tag_block animations;                      /* 0x74 */
} animation_graph;                              /* 128 bytes */
