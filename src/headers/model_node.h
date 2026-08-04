#pragma once
/* model_node — one node (bone) definition within a model's node hierarchy (156 bytes, DB layout). */

#include "real_point3d.h"
#include "real_quaternion.h"
#include "real_matrix4x3.h"

typedef struct model_node
{
    char             name[32];                       /* 0x00 */
    __int16          next_sibling_node_index;         /* 0x20 */
    __int16          first_child_node_index;          /* 0x22 */
    __int16          parent_node_index;                /* 0x24 */
    unsigned __int16 pad;                              /* 0x26 */
    real_point3d     default_translation;             /* 0x28 */
    real_quaternion  default_rotation;                 /* 0x34 */
    float            node_distance_from_parent;        /* 0x44 */
    int              unused[8];                        /* 0x48 */
    real_matrix4x3   runtime_default_inverse_matrix;   /* 0x68 */
} model_node;                                           /* 156 bytes */
