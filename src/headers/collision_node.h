#pragma once
/* collision_node — one node of a collision model's node tree (64 bytes; layout from the database). */

#include "tag_block.h"

typedef struct collision_node
{
    char             name[32];                /* 0x00 */
    __int16          region_index;            /* 0x20 */
    __int16          parent_node_index;       /* 0x22 */
    __int16          next_sibling_node_index; /* 0x24 */
    __int16          first_child_node_index;  /* 0x26 */
    int              unused[2];               /* 0x28 */
    unsigned __int16 pad;                     /* 0x30 */
    __int16          runtime_damage_part;     /* 0x32 */
    tag_block        bsps;                    /* 0x34 */
} collision_node;                             /* 64 bytes */
