#pragma once
/* animation_graph_weapon_type — one element of an animation_graph_weapon_class's weapon_types tag_block
 * (60 bytes): a weapon type's label and its animation index table. Layout DB-verified (2026-07-14). */

#include "tag_block.h"

typedef struct animation_graph_weapon_type
{
    char      label[32];    /* 0x00 */
    int       unused[4];    /* 0x20 (32) */
    tag_block animations;   /* 0x30 (48) */
} animation_graph_weapon_type;  /* 60 bytes */
