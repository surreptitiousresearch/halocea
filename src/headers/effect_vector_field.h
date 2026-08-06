#pragma once
/* effect_vector_field — optional directional impulse field applied to a spawned effect (effects.c).
 * Layout from the database. */

#include "real_vector3d.h"
#include "real_point3d.h"

typedef struct effect_vector_field
{
    void *identifier;                                                          /* 0x00 */
    void (*translational)(real_vector3d *, const real_point3d *, void *); /* 0x04 */
    void (*angular)(real_vector3d *, const real_point3d *, void *);                  /* 0x08 */
} effect_vector_field;                                                          /* 12 bytes */
