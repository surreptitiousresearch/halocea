#pragma once
/* animation_graph_sound_reference — one element of animation_graph.sound_references
   (20 bytes; DB-verified). */

#include "tag_reference.h"

typedef struct animation_graph_sound_reference
{
    tag_reference sound;        /* 0x00 */
    int           crazy_unused; /* 0x10 */
} animation_graph_sound_reference; /* 20 bytes */
