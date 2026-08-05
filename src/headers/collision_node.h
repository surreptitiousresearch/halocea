#pragma once
/* collision_node — one node of a collision model's node tree (64 bytes; layout from the database). */

#include <stdint.h>
#include "tag_block.h"

typedef struct collision_node
{
    char             name[32];                /* 0x00 */
    int16_t          region_index;            /* 0x20 */
    int16_t          parent_node_index;       /* 0x22 */
    int16_t          next_sibling_node_index; /* 0x24 */
    int16_t          first_child_node_index;  /* 0x26 */
    int              unused[2];               /* 0x28 */
    uint16_t pad;                     /* 0x30 */
    int16_t          runtime_damage_part;     /* 0x32 */
    tag_block        bsps;                    /* 0x34 */
} collision_node;                             /* 64 bytes */
