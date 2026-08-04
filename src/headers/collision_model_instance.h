/* collision_model_instance — a resolved object's collision model bound to its current node matrices
 * and region/permutation state, used to test rays against the object (16 bytes). */
#pragma once

#include "collision_model.h"
#include "real_matrix4x3.h"

typedef struct collision_model_instance
{
    int                          object_index;               /* 0x00 */
    const collision_model       *model;                      /* 0x04 */
    const unsigned char         *region_permutation_indices; /* 0x08 */
    const real_matrix4x3        *matrices;                   /* 0x0C */
} collision_model_instance;
