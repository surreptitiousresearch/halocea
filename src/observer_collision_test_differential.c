/* observer_collision_test_differential @0x8370F580 — DEVIATION: the decompiler fabricated a bogus 7th `a7`
 * parameter and confusingly renamed the true 5th/6th params in its call to observer_collision_test_with_t.
 * The DB's own funcs.prototype lists exactly 6 parameters (origin, destination, differential_basis,
 * differential, t, ignore_media), and disasm_range(0x8370F580,0x8370F5D4) confirms `t` (r7) and
 * `ignore_media` (r8) are passed straight through unchanged — reproduced with the true 6-arg signature. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern uint8_t observer_collision_test_with_t(const real_point3d *p0, const real_point3d *p1, float *t, uint8_t ignore_media);

uint8_t observer_collision_test_differential(
    const real_point3d *origin,
    const real_point3d *destination,
    const real_vector3d *differential_basis,
    float differential,
    float *t,
    uint8_t ignore_media)
{
    real_point3d computed_destination;

    computed_destination.n[0] = differential_basis->n[0] * differential + destination->n[0];
    computed_destination.n[1] = differential_basis->n[1] * differential + destination->n[1];
    computed_destination.n[2] = differential_basis->n[2] * differential + destination->n[2];

    return observer_collision_test_with_t(origin, &computed_destination, t, ignore_media);
}
