/* valid_real_vector3d_axes3 @0x83819D30 — validate that three vectors form an orthonormal basis: each must
 * be a valid unit normal, and all three pairwise dot products must be ~0 (mutually orthogonal). */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern uint8_t valid_real_normal3d(const real_vector3d *n);
extern uint8_t valid_realcmp(float x, float y);

uint8_t valid_real_vector3d_axes3(const real_vector3d *forward, const real_vector3d *left, const real_vector3d *up)
{
    if (!valid_real_normal3d(forward))
        return 0;
    if (!valid_real_normal3d(left))
        return 0;
    if (!valid_real_normal3d(up))
        return 0;

    if (!valid_realcmp(left->n[1] * forward->n[1] + (forward->n[0] * left->n[0] + left->n[2] * forward->n[2]),
                       0.0f))
        return 0;
    if (!valid_realcmp(left->n[1] * up->n[1] + (left->n[0] * up->n[0] + left->n[2] * up->n[2]), 0.0f))
        return 0;
    if (!valid_realcmp(forward->n[2] * up->n[2] + (forward->n[1] * up->n[1] + forward->n[0] * up->n[0]), 0.0f))
        return 0;

    return 1;
}
