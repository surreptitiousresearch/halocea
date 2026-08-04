#pragma once
/* animation_graph_weapon_animations — one element of animation_graph's weapon_animations block:
   a nested block of per-state animation indices (__int16). 28 bytes, layout from the database. */

#include "tag_block.h"

typedef struct animation_graph_weapon_animations
{
    int       unused1[4]; /* 0x00 */
    tag_block animations; /* 0x10 — elements are __int16 animation indices */
} animation_graph_weapon_animations; /* 28 bytes */
