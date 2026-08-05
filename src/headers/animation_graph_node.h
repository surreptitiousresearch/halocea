#pragma once
/* animation_graph_node — one node (bone) definition within an animation graph's node hierarchy
 * (64 bytes, DB layout). */

#include <stdint.h>
#include "real_vector3d.h"

typedef struct animation_graph_node
{
    char             name[32];                /* 0x00 */
    int16_t          next_sibling_node_index;  /* 0x20 */
    int16_t          first_child_node_index;   /* 0x22 */
    int16_t          parent_node_index;        /* 0x24 */
    uint16_t pad;                      /* 0x26 */
    unsigned int     flags;                    /* 0x28 */
    real_vector3d    base_vector;              /* 0x2C */
    float            range;                    /* 0x38 */
    int              pad1;                     /* 0x3C */
} animation_graph_node;                         /* 64 bytes */
