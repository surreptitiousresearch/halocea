/* pin_normal_to_cone3d @0x836FCA60 — if `normal` already lies within `cosine` of `direction` (dot >=
 * cosine), copies `normal` straight to `result` and returns 0 (no adjustment needed). Otherwise "pins"
 * `direction` onto the cone's surface at the azimuth nearest `normal`: builds a rotation axis from
 * `direction` x `normal` (falling back to any perpendicular of `direction` if that cross product
 * degenerates, i.e. `direction`/`normal` are parallel), then rotates `direction` by the angle whose
 * sine/cosine are given (the cone's half-angle) around that axis via the standard Rodrigues rotation
 * formula, writing the result to `result`, and returns 1.
 *
 * DEVIATION: the DB prototype confirms only 5 real parameters (both `sine` and `cosine` are `float`, not
 * `double`) — the raw decompile fabricated phantom `a6`/`a7` parameters and, confusingly, never used its
 * own (equally phantom) `result` parameter at all: disasm confirms the true 5th parameter arrives in r7
 * (both float parameters each reserve their own now-familiar ABI shadow GPR slot, pushing `result` from r5
 * to r7), matching every write the raw decompile attributed to `a7`. The tail expression was expanded
 * Rodrigues rotation math — reproduced here in its closed form after verifying the z-component term by term
 * against the raw decompile's expanded form. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern float normalize3d(real_vector3d *v);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);

uint8_t pin_normal_to_cone3d(const real_vector3d *normal, const real_vector3d *direction, float sine,
        float cosine, real_vector3d *result)
{
    float dot = direction->n[0] * normal->n[0] + normal->n[1] * direction->n[1] + direction->n[2] * normal->n[2];

    if ( dot >= cosine )
    {
        *result = *normal;
        return 0;
    }

    real_vector3d axis;
    axis.n[0] = direction->n[1] * normal->n[2] - direction->n[2] * normal->n[1];
    axis.n[1] = direction->n[2] * normal->n[0] - normal->n[2] * direction->n[0];
    axis.n[2] = normal->n[1] * direction->n[0] - direction->n[1] * normal->n[0];

    if ( normalize3d(&axis) == 0.0f )
    {
        perpendicular3d(direction, &axis);
        normalize3d(&axis);
    }

    float axis_dot_direction = axis.n[0] * direction->n[0] + axis.n[1] * direction->n[1]
            + axis.n[2] * direction->n[2];
    float scaled = axis_dot_direction * (1.0f - cosine);

    result->n[0] = direction->n[0] * cosine + (axis.n[1] * direction->n[2] - axis.n[2] * direction->n[1]) * sine
            + axis.n[0] * scaled;
    result->n[1] = direction->n[1] * cosine + (axis.n[2] * direction->n[0] - axis.n[0] * direction->n[2]) * sine
            + axis.n[1] * scaled;
    result->n[2] = direction->n[2] * cosine + (axis.n[0] * direction->n[1] - axis.n[1] * direction->n[0]) * sine
            + axis.n[2] * scaled;

    return 1;
}
