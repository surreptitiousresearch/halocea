/* physics_instance — a resolved object's physics definition bound to its current world matrix, used
 * to test rays against the object's physics hull (60 bytes). */
#pragma once

#include "real_matrix4x3.h"

struct physics_definition;

typedef struct physics_instance
{
    int                              object_index; /* 0x00 */
    const struct physics_definition *physics;      /* 0x04 */
    real_matrix4x3                   world_matrix; /* 0x08 */
} physics_instance;
