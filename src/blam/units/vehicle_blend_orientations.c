/* vehicle_blend_orientations @0x8375D028 — blend two (forward, up) orientation bases by blend_scale,
 * writing the interpolated-and-normalized result into forward_out/up_out, then re-orthogonalizes up_out
 * against forward_out via a double cross product (side = forward_out x up_out; up_out = forward_out x
 * side) so the output stays a valid orthonormal basis. Bails out (returns 0, leaving *forward_out/*up_out
 * untouched) if either input pair is nearly opposed (dot <= -0.9), since interpolation would be
 * ill-defined there.
 *
 * DEVIATION: the decompiler renders this with a phantom 8th parameter ("a8") and an uninitialized local
 * ("v13") passed as the up-interpolation's result pointer; the DB's real 7-parameter prototype
 * (forward_1, up_1, forward_2, up_2, blend_scale, forward_out, up_out) is ground truth, confirmed via
 * full disasm register trace — both interpolate calls write into forward_out/up_out respectively (no
 * separate output vector exists), and the final cross-product block reads and rewrites up_out in place. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern void vectors_interpolate(const real_vector3d *a, const real_vector3d *b, float t, real_vector3d *result);
extern float normalize3d(real_vector3d *v);

uint8_t vehicle_blend_orientations(const real_vector3d *forward_1, const real_vector3d *up_1, const real_vector3d *forward_2, const real_vector3d *up_2, float blend_scale, real_vector3d *forward_out, real_vector3d *up_out)
{
    float forward_dot = forward_1->n[0] * forward_2->n[0] + forward_1->n[1] * forward_2->n[1]
                       + forward_1->n[2] * forward_2->n[2];
    if ( forward_dot <= -0.9f )
        return 0;

    vectors_interpolate(forward_1, forward_2, blend_scale, forward_out);
    normalize3d(forward_out);

    float up_dot = up_1->n[0] * up_2->n[0] + up_1->n[1] * up_2->n[1] + up_1->n[2] * up_2->n[2];
    if ( up_dot <= -0.9f )
        return 0;

    vectors_interpolate(up_1, up_2, blend_scale, up_out);
    normalize3d(up_out);

    float side_x = forward_out->n[1] * up_out->n[2] - forward_out->n[2] * up_out->n[1];
    float side_y = forward_out->n[2] * up_out->n[0] - forward_out->n[0] * up_out->n[2];
    float side_z = forward_out->n[0] * up_out->n[1] - forward_out->n[1] * up_out->n[0];

    up_out->n[2] = forward_out->n[1] * side_x - forward_out->n[0] * side_y;
    up_out->n[1] = forward_out->n[0] * side_z - forward_out->n[2] * side_x;
    up_out->n[0] = forward_out->n[2] * side_y - forward_out->n[1] * side_z;

    normalize3d(up_out);
    return 1;
}
