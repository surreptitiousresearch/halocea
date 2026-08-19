/* valid_real_vector3d_axes2 @0x8370F6D8 — true when f and u are each valid unit normals AND orthogonal (their
 * dot product is within 0.001 of 0.0). Two-axis counterpart of the full three-axis orthonormal-basis
 * validator (see valid_real_normal3d.c / valid_realcmp.c). */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern uint8_t valid_real_normal3d(const real_vector3d *n);
extern uint8_t valid_realcmp(float x, float y);

uint8_t valid_real_vector3d_axes2(const real_vector3d *f, const real_vector3d *u)
{
    if ( !valid_real_normal3d(f) )
        return 0;
    if ( !valid_real_normal3d(u) )
        return 0;

    float dot = f->n[0] * u->n[0] + (f->n[2] * u->n[2] + f->n[1] * u->n[1]);
    return valid_realcmp(dot, 0.0f);
}
