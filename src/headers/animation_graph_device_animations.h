#pragma once
/* animation_graph_device_animations — one per-device selector record in an animation graph's
 * device_animations block. A leading reserved region followed by an "animations" tag_block whose
 * address is an array of animation_graph_animation_index (__int16) entries. */

#include "tag_block.h"

typedef struct animation_graph_device_animations
{
    int unused[21];        /* 0x00 */
    tag_block animations;  /* 0x54 — array of animation_graph_animation_index (__int16) */
} animation_graph_device_animations;
