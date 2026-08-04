#pragma once
/* vector_avoidance_ray — one parameterized avoidance ray used by the AI vector-avoidance pass: a length scale
 * plus an offset (start displacement) and divergence (direction spread) applied in the mover's local frame.
 * Offsets from disasm (0x837C7FC8-0x837C80CC). */

#include "real_vector3d.h"

typedef struct vector_avoidance_ray
{
    float         length;      /* 0x00 */
    real_vector3d offset;      /* 0x04 */  /* recovered: real_point3d -> real_vector3d (ref/DB) */
    real_vector3d divergence;  /* 0x10 */
} vector_avoidance_ray;        /* 0x1C (28 bytes) */
