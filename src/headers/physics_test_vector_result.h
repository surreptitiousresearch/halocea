/* physics_test_vector_result — nearest hit of a ray against an object's physics hull (20 bytes). */
#pragma once

#include "real_plane3d.h"

typedef struct physics_test_vector_result
{
    float        t;     /* 0x00 */
    real_plane3d plane; /* 0x04 */
} physics_test_vector_result;
